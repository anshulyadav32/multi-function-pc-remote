// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

import Foundation
import UIKit

class ConnectionManager: NSObject, ObservableObject, URLSessionWebSocketDelegate {
    @Published var isConnected = false
    
    private var webSocketTask: URLSessionWebSocketTask?
    private var urlSession: URLSession?
    
    override init() {
        super.init()
        let configuration = URLSessionConfiguration.default
        urlSession = URLSession(configuration: configuration, delegate: self, delegateQueue: OperationQueue())
    }
    
    func connect(to address: String, completion: @escaping (Bool, String?) -> Void) {
        guard let url = URL(string: "ws://\(address)") else {
            completion(false, "Invalid server address")
            return
        }
        
        webSocketTask = urlSession?.webSocketTask(with: url)
        webSocketTask?.resume()
        
        receiveMessage()
        
        DispatchQueue.main.asyncAfter(deadline: .now() + 1.0) {
            if self.webSocketTask?.state == .running {
                self.isConnected = true
                completion(true, nil)
            } else {
                completion(false, "Connection failed")
            }
        }
    }
    
    func disconnect() {
        webSocketTask?.cancel(with: .goingAway, reason: nil)
        webSocketTask = nil
        isConnected = false
    }
    
    func sendCommand(type: String, action: String, data: [String: Any] = [:]) {
        var command: [String: Any] = [
            "type": type,
            "action": action,
            "id": Int(Date().timeIntervalSince1970 * 1000)
        ]
        
        // Merge additional data
        for (key, value) in data {
            command[key] = value
        }
        
        guard let jsonData = try? JSONSerialization.data(withJSONObject: command),
              let jsonString = String(data: jsonData, encoding: .utf8) else {
            return
        }
        
        let message = URLSessionWebSocketTask.Message.string(jsonString)
        webSocketTask?.send(message) { error in
            if let error = error {
                print("WebSocket send error: \(error)")
            }
        }
    }
    
    private func receiveMessage() {
        webSocketTask?.receive { [weak self] result in
            switch result {
            case .success(let message):
                switch message {
                case .string(let text):
                    self?.handleMessage(text)
                case .data(let data):
                    if let text = String(data: data, encoding: .utf8) {
                        self?.handleMessage(text)
                    }
                @unknown default:
                    break
                }
                
                // Continue receiving
                self?.receiveMessage()
                
            case .failure(let error):
                print("WebSocket receive error: \(error)")
                DispatchQueue.main.async {
                    self?.isConnected = false
                }
            }
        }
    }
    
    private func handleMessage(_ message: String) {
        guard let data = message.data(using: .utf8),
              let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
            return
        }
        
        if let type = json["type"] as? String,
           type == "screen",
           let action = json["action"] as? String,
           action == "frame",
           let base64Data = json["data"] as? String,
           let imageData = Data(base64Encoded: base64Data),
           let image = UIImage(data: imageData) {
            
            DispatchQueue.main.async {
                NotificationCenter.default.post(
                    name: .screenFrameReceived,
                    object: image
                )
            }
        }
    }
    
    // URLSessionWebSocketDelegate
    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didOpenWithProtocol protocol: String?) {
        DispatchQueue.main.async {
            self.isConnected = true
        }
    }
    
    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didCloseWith closeCode: URLSessionWebSocketTask.CloseCode, reason: Data?) {
        DispatchQueue.main.async {
            self.isConnected = false
        }
    }
}
