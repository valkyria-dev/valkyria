/*
 * Copyright (c) 2024 Valkyria.dev
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

// HttpServer.h
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>
#include <iostream>
#include <vector>
#include <mutex>

struct Route {
    std::string path;
    std::string content;
};

struct Config {
    int port;
    std::string rootPath;
    std::vector<Route> routes;
    bool interactiveMode;
};

bool parseXmlConfig(const std::string& filePath, Config& config);

struct HttpRequest {
    std::string method;
    std::string path;
};

struct HttpResponse {
    int status;
    std::string body;
};

class SimpleHttpServer {
public:
    SimpleHttpServer(const Config& cfg);

    void start();

private:
    int port;
    std::vector<Route> routes;
    bool interactiveMode;

    int createServerSocket();
    int acceptConnection(int serverSocket);
    void handleRequest(int clientSocket);
    void parseHttpRequest(const char* buffer, HttpRequest& request);
    void handleRoute(const HttpRequest& request, HttpResponse& response);
    void sendHttpResponse(int clientSocket, const HttpResponse& response);
};

void handleSignal(int signal);
void interactiveConsole(Config& config);

#endif  // HTTP_SERVER_
