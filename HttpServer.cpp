/*
 * Copyright (c) 2011 Valkyria.dev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// HttpServer.cpp
#include <unistd.h>
#include "HttpServer.h"
#include <csignal>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>

volatile sig_atomic_t gSignalReceived = 0;
bool gInteractiveConsoleActive = true;
std::mutex gConsoleMutex;

void handleSignal(int signal) {
    if (signal == SIGINT) {
        gSignalReceived = 1;
    }
}

void interactiveConsole(Config& config) {
    std::string command;
    while (gInteractiveConsoleActive) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "stop") {
            {
                std::lock_guard<std::mutex> lock(gConsoleMutex);
                gInteractiveConsoleActive = false;
            }
            std::cout << "Stopping server..." << std::endl;
            gSignalReceived = 1;
            break;
        } else if (command == "restart") {
            std::cout << "Restarting server..." << std::endl;
            gSignalReceived = 1;
            break;
        } else {
            std::cout << "Unknown command. Available commands: stop, restart." << std::endl;
        }
    }
    std::cout << "Interactive console thread finished." << std::endl;
}

bool parseXmlConfig(const std::string& filePath, Config& config) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening XML file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    std::stringstream buffer;

    while (std::getline(file, line)) {
        buffer << line;
    }

    file.close();

    std::string xmlContent = buffer.str();

    size_t pos = 0;
    while ((pos = xmlContent.find("<route path=")) != std::string::npos) {
        size_t start = pos + 13;
        size_t end = xmlContent.find("\"", start + 1);
        size_t pathLength = end - start;
        std::string path = xmlContent.substr(start, pathLength);

        pos = xmlContent.find("content=\"", end);
        start = pos + 9;
        end = xmlContent.find("\"", start + 1);
        size_t contentLength = end - start;
        std::string content = xmlContent.substr(start, contentLength);

        config.routes.push_back({path, content});
    }

    pos = xmlContent.find("<port>");
    if (pos != std::string::npos) {
        size_t start = pos + 6;
        size_t end = xmlContent.find("</port>", start + 1);
        size_t portLength = end - start;
        std::string portStr = xmlContent.substr(start, portLength);
        config.port = std::stoi(portStr);
    }

    pos = xmlContent.find("<rootPath>");
    if (pos != std::string::npos) {
        size_t start = pos + 10;
        size_t end = xmlContent.find("</rootPath>", start + 1);
        size_t rootPathLength = end - start;
        config.rootPath = xmlContent.substr(start, rootPathLength);
    }

    pos = xmlContent.find("<interactiveMode>");
    if (pos != std::string::npos) {
        size_t start = pos + 17;
        size_t end = xmlContent.find("</interactiveMode>", start + 1);
        size_t modeLength = end - start;
        std::string modeStr = xmlContent.substr(start, modeLength);
        config.interactiveMode = (modeStr == "true" || modeStr == "1");
    }

    return true;
}

SimpleHttpServer::SimpleHttpServer(const Config& cfg)
    : port(cfg.port), routes(cfg.routes), interactiveMode(cfg.interactiveMode) {}

void SimpleHttpServer::start() {
    std::cout << "Starting server..." << std::endl;

    int serverSocket = createServerSocket();
    if (serverSocket < 0) {
        std::cerr << "Error creating server socket." << std::endl;
        return;
    }

    while (!gSignalReceived) {
        int clientSocket = acceptConnection(serverSocket);
        if (clientSocket < 0) {
            std::cerr << "Error accepting connection." << std::endl;
            continue;
        }

        // Imprimimos un mensaje para saber que hemos aceptado una conexiÃ³n
        std::cout << "Accepted a connection." << std::endl;

        std::thread([this, clientSocket]() {
            handleRequest(clientSocket);
            close(clientSocket);
        }).detach();
    }

    close(serverSocket);

    std::cout << "Server stopped." << std::endl;
