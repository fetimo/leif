//
//  leifApp.swift
//  leif
//
//  Created by Tim Stone on 05/07/2022.
//

import SwiftUI

@main
struct swiftui_menu_barApp: App {
    // 1
    @State var currentNumber: String = "1"
    
    var body: some Scene {
        
        // 2
        MenuBarExtra(currentNumber, systemImage: "\(currentNumber).circle") {
            // 3
            Button("One") {
                currentNumber = "1"
            }.keyboardShortcut("1")
            Button("Two") {
                currentNumber = "2"
            }.keyboardShortcut("2")
            Button("Three") {
                currentNumber = "3"
            }.keyboardShortcut("3")
            Divider()

            Button("Quit") {

                NSApplication.shared.terminate(nil)

            }.keyboardShortcut("q")
        }
    }
}
