// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

import SwiftUI

struct ScreenShareView: View {
    @EnvironmentObject var connectionManager: ConnectionManager
    @State private var isStreaming = false
    @State private var screenImage: UIImage?
    
    var body: some View {
        VStack(spacing: 20) {
            Text("Screen Sharing")
                .font(.title2)
                .fontWeight(.bold)
                .padding(.top)
            
            if isStreaming {
                if let image = screenImage {
                    Image(uiImage: image)
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                        .cornerRadius(12)
                        .padding()
                } else {
                    VStack {
                        ProgressView()
                            .scaleEffect(1.5)
                        Text("Waiting for screen data...")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                            .padding(.top)
                    }
                    .frame(maxWidth: .infinity, maxHeight: 300)
                    .background(Color.gray.opacity(0.1))
                    .cornerRadius(12)
                    .padding()
                }
                
                Button(action: stopScreenShare) {
                    HStack {
                        Image(systemName: "stop.circle.fill")
                        Text("Stop Sharing")
                            .fontWeight(.semibold)
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.red)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                }
                .padding()
            } else {
                Spacer()
                
                Image(systemName: "display")
                    .font(.system(size: 80))
                    .foregroundColor(.blue)
                
                Text("View your PC screen remotely")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                    .padding()
                
                Button(action: startScreenShare) {
                    HStack {
                        Image(systemName: "play.circle.fill")
                        Text("Start Screen Sharing")
                            .fontWeight(.semibold)
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.blue)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                }
                .padding()
                
                Spacer()
            }
        }
        .onReceive(NotificationCenter.default.publisher(for: .screenFrameReceived)) { notification in
            if let image = notification.object as? UIImage {
                screenImage = image
            }
        }
    }
    
    private func startScreenShare() {
        isStreaming = true
        connectionManager.sendCommand(type: "screen", action: "start")
    }
    
    private func stopScreenShare() {
        isStreaming = false
        screenImage = nil
        connectionManager.sendCommand(type: "screen", action: "stop")
    }
}

extension Notification.Name {
    static let screenFrameReceived = Notification.Name("screenFrameReceived")
}

struct ScreenShareView_Previews: PreviewProvider {
    static var previews: some View {
        ScreenShareView()
            .environmentObject(ConnectionManager())
    }
}
