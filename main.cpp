#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>

using namespace std;

struct HttpRequest {
    string method;
    string path;
};

struct HttpResponse {
    int status;
    string body;
};

struct Route {
    string path;
    string content;
};

struct Config {
    int port;
    string rootPath;
    vector<Route> routes;
    bool interactiveMode;
};

// Variable global para indicar si se ha recibido la señal de interrupción
volatile sig_atomic_t gSignalReceived = 0;

// Variable global para la consola interactiva
bool gInteractiveConsoleActive = true;

// Mutex para sincronizar el acceso a la consola interactiva
std::mutex gConsoleMutex;

// Función para manejar la señal SIGINT (Ctrl+C)
void handleSignal(int signal) {
    if (signal == SIGINT) {
        gSignalReceived = 1;
    }
}

// Función para la consola interactiva
void interactiveConsole(Config& config) {
    string command;
    while (gInteractiveConsoleActive) {
        cout << "> ";
        getline(cin, command);

        if (command == "stop") {
            {
                std::lock_guard<std::mutex> lock(gConsoleMutex);
                gInteractiveConsoleActive = false;
            }
            cout << "Stopping server..." << endl;
            gSignalReceived = 1;
            break;
        } else if (command == "restart") {
            cout << "Restarting server..." << endl;
            gSignalReceived = 1;
            break;
        } else {
            cout << "Unknown command. Available commands: stop, restart." << endl;
        }
    }
}

bool parseXmlConfig(const string& filePath, Config& config) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening XML file: " << filePath << endl;
        return false;
    }

    string line;
    stringstream buffer;

    while (getline(file, line)) {
        buffer << line;
    }

    file.close();

    string xmlContent = buffer.str();

    size_t pos = 0;
    while ((pos = xmlContent.find("<route path=")) != string::npos) {
        size_t start = pos + 13;
        size_t end = xmlContent.find("\"", start + 1);
        size_t pathLength = end - start;
        string path = xmlContent.substr(start, pathLength);

        pos = xmlContent.find("content=\"", end);
        start = pos + 9;
        end = xmlContent.find("\"", start + 1);
        size_t contentLength = end - start;
        string content = xmlContent.substr(start, contentLength);

        config.routes.push_back({path, content});
    }

    pos = xmlContent.find("<port>");
    if (pos != string::npos) {
        size_t start = pos + 6;
        size_t end = xmlContent.find("</port>", start + 1);
        size_t portLength = end - start;
        string portStr = xmlContent.substr(start, portLength);
        config.port = stoi(portStr);
    }

    pos = xmlContent.find("<rootPath>");
    if (pos != string::npos) {
        size_t start = pos + 10;
        size_t end = xmlContent.find("</rootPath>", start + 1);
        size_t rootPathLength = end - start;
        config.rootPath = xmlContent.substr(start, rootPathLength);
    }

    pos = xmlContent.find("<interactiveMode>");
    if (pos != string::npos) {
        size_t start = pos + 17;
        size_t end = xmlContent.find("</interactiveMode>", start + 1);
        size_t modeLength = end - start;
        string modeStr = xmlContent.substr(start, modeLength);
        config.interactiveMode = (modeStr == "true" || modeStr == "1");
    }

    return true;
}

class SimpleHttpServer {
public:
    explicit SimpleHttpServer(const Config& config) : port(config.port), routes(config.routes), interactiveMode(config.interactiveMode) {}

    void start() {
        // Configura el manejador de señales para SIGINT
        signal(SIGINT, handleSignal);

        int serverSocket = createServerSocket();
        if (serverSocket == -1) {
            cerr << "Error creating server socket." << endl;
            return;
        }

        cout << "Server listening on port " << port << endl;

        if (interactiveMode) {
            // Inicia la consola interactiva en un hilo separado
            std::thread consoleThread(interactiveConsole, std::ref(config));
            // Espera a que termine la consola interactiva
            consoleThread.join();
        } else {
            while (!gSignalReceived) {
                int clientSocket = acceptConnection(serverSocket);
                if (clientSocket == -1) {
                    cerr << "Error accepting connection." << endl;
                    continue;
                }

                handleRequest(clientSocket);

                close(clientSocket);
            }
        }

        // Cierra el socket del servidor antes de salir
        close(serverSocket);
        cout << "Server closed." << endl;
    }

private:
    int port;
    vector<Route> routes;
    bool interactiveMode;

    int createServerSocket() {
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            cerr << "Error creating socket." << endl;
            return -1;
        }

        sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
            cerr << "Error binding socket." << endl;
            close(serverSocket);
            return -1;
        }

        if (listen(serverSocket, 5) == -1) {
            cerr << "Error listening on socket." << endl;
            close(serverSocket);
            return -1;
        }

        return serverSocket;
    }

    int acceptConnection(int serverSocket) {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        return accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
    }

    void handleRequest(int clientSocket) {
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));

        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            cerr << "Error reading from socket." << endl;
            return;
        }

        HttpRequest request;
        parseHttpRequest(buffer, request);

        cout << "Received request: " << request.method << " " << request.path << endl;

        HttpResponse response;

        // Handle request based on path
        handleRoute(request, response);

        sendHttpResponse(clientSocket, response);
    }

    void parseHttpRequest(const char* buffer, HttpRequest& request) {
        // Implementa la lógica para analizar la solicitud HTTP según tus necesidades
        char* bufferCopy = strdup(buffer);  // Duplica el buffer ya que strtok modifica el string
        const char* method = strtok(bufferCopy, " ");
        const char* path = strtok(nullptr, " ");

        if (method && path) {
            request.method = method;
            request.path = path;
        }

        free(bufferCopy);  // Libera la memoria del buffer duplicado
    }

    void handleRoute(const HttpRequest& request, HttpResponse& response) {
        // Implementa la lógica para manejar las rutas según tus necesidades
        for (const auto& route : routes) {
            if (request.path == route.path) {
                response.status = 200;
                response.body = route.content;
                return;
            }
        }

        response.status = 404;
        response.body = "<h1>404 Not Found</h1>";
    }

    void sendHttpResponse(int clientSocket, const HttpResponse& response) {
        ostringstream oss;
        oss << "HTTP/1.1 " << response.status << " OK\r\n"
            << "Content-Length: " << response.body.length() << "\r\n"
            << "Content-Type: text/html\r\n"
            << "\r\n"
            << response.body;

        send(clientSocket, oss.str().c_str(), oss.str().length(), 0);
    }
};

int main() {
    Config config;
    if (parseXmlConfig("config.xml", config)) {
        SimpleHttpServer server(config);
        server.start();
    }

    return 0;
}
