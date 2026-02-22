// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 Multi-Function PC Remote Contributors

import SwiftUI

struct InputControlView: View {
    @EnvironmentObject var connectionManager: ConnectionManager
    @State private var dragOffset = CGSize.zero
    @State private var lastPosition = CGPoint.zero
    
    var body: some View {
        VStack(spacing: 24) {
            Text("Touchpad")
                .font(.title2)
                .fontWeight(.bold)
                .padding(.top)
            
            Text("Drag to move the mouse")
                .font(.subheadline)
                .foregroundColor(.secondary)
            
            // Touchpad area
            RoundedRectangle(cornerRadius: 20)
                .fill(Color.gray.opacity(0.2))
                .frame(height: 350)
                .overlay(
                    VStack {
                        Image(systemName: "hand.point.up.left.fill")
                            .font(.system(size: 60))
                            .foregroundColor(.gray)
                        Text("Touch here")
                            .font(.headline)
                            .foregroundColor(.gray)
                    }
                )
                .gesture(
                    DragGesture()
                        .onChanged { gesture in
                            let deltaX = Int(gesture.location.x - lastPosition.x)
                            let deltaY = Int(gesture.location.y - lastPosition.y)
                            
                            if abs(deltaX) > 0 || abs(deltaY) > 0 {
                                connectionManager.sendCommand(
                                    type: "input",
                                    action: "mouse_move",
                                    data: ["deltaX": deltaX, "deltaY": deltaY]
                                )
                            }
                            
                            lastPosition = gesture.location
                        }
                        .onEnded { _ in
                            lastPosition = .zero
                        }
                )
                .padding()
            
            // Mouse buttons
            HStack(spacing: 20) {
                Button(action: { sendMouseClick("left") }) {
                    VStack {
                        Image(systemName: "hand.tap.fill")
                            .font(.system(size: 30))
                        Text("Left Click")
                            .font(.caption)
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.blue)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                }
                
                Button(action: { sendMouseClick("right") }) {
                    VStack {
                        Image(systemName: "hand.tap.fill")
                            .font(.system(size: 30))
                        Text("Right Click")
                            .font(.caption)
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.blue)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                }
            }
            .padding(.horizontal)
            
            Spacer()
        }
    }
    
    private func sendMouseClick(_ button: String) {
        connectionManager.sendCommand(
            type: "input",
            action: "mouse_click",
            data: ["button": button]
        )
    }
}

struct InputControlView_Previews: PreviewProvider {
    static var previews: some View {
        InputControlView()
            .environmentObject(ConnectionManager())
    }
}
