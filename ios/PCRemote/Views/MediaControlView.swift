// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

import SwiftUI

struct MediaControlView: View {
    @EnvironmentObject var connectionManager: ConnectionManager
    
    var body: some View {
        VStack(spacing: 32) {
            Spacer()
            
            // Album art placeholder
            RoundedRectangle(cornerRadius: 20)
                .fill(Color.gray.opacity(0.3))
                .frame(width: 250, height: 250)
                .overlay(
                    Image(systemName: "music.note")
                        .font(.system(size: 80))
                        .foregroundColor(.gray)
                )
            
            Text("Media Controls")
                .font(.title2)
                .fontWeight(.bold)
            
            // Playback controls
            HStack(spacing: 40) {
                Button(action: { sendMediaCommand("previous") }) {
                    Image(systemName: "backward.fill")
                        .font(.system(size: 40))
                        .foregroundColor(.blue)
                }
                
                Button(action: { sendMediaCommand("play_pause") }) {
                    Image(systemName: "play.circle.fill")
                        .font(.system(size: 70))
                        .foregroundColor(.blue)
                }
                
                Button(action: { sendMediaCommand("next") }) {
                    Image(systemName: "forward.fill")
                        .font(.system(size: 40))
                        .foregroundColor(.blue)
                }
            }
            .padding()
            
            // Volume controls
            HStack(spacing: 20) {
                Button(action: { sendMediaCommand("mute") }) {
                    VStack {
                        Image(systemName: "speaker.slash.fill")
                            .font(.system(size: 30))
                        Text("Mute")
                            .font(.caption)
                    }
                    .foregroundColor(.blue)
                }
            }
            
            Spacer()
        }
        .padding()
    }
    
    private func sendMediaCommand(_ action: String) {
        connectionManager.sendCommand(type: "media", action: action)
    }
}

struct MediaControlView_Previews: PreviewProvider {
    static var previews: some View {
        MediaControlView()
            .environmentObject(ConnectionManager())
    }
}
