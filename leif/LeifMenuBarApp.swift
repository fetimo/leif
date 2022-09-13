//
//  LeifMenuBarApp.swift
//  leif
//
//  Created by Tim Stone on 05/07/2022.
//

import SwiftUI
import Foundation
import Cocoa

@main
struct LeifMenuBarApp: App {

    @NSApplicationDelegateAdaptor(AppDelegate.self) private var appDelegate

    var body: some Scene {
        WindowGroup {
            ContentView(vm: LeifViewModel()).frame(width: .zero)
        }
    }
}


class AppDelegate: NSObject, NSApplicationDelegate, ObservableObject {
    
    var statusItem: NSStatusItem!
    let unit = "gCO₂eq"
    
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        statusItem = NSStatusBar.system.statusItem(withLength: NSStatusItem.variableLength)
        setupMenus()
    }
    
    func setupMenus() {
        let menu = NSMenu()
        menu.autoenablesItems = false
        
        let session = NSMenuItem(title: "Session: 0.0 \(unit)", action: #selector(noop), keyEquivalent: "")
        session.isEnabled = false
        menu.addItem(session)
        
        let total = NSMenuItem(title: "Lifetime: 0.0 \(unit)", action: #selector(noop) , keyEquivalent: "")
        total.isEnabled = false
        menu.addItem(total)
        
        let intensity = NSMenuItem(title: "Current intensity: unknown", action: #selector(noop) , keyEquivalent: "")
        intensity.isEnabled = false
        menu.addItem(intensity)
        
        let intel = NSMenuItem(title: "Intel: unknown", action: #selector(noop) , keyEquivalent: "")
        intel.isEnabled = false
        menu.addItem(intel)
        
        // Setup Regions submenu
        let regionMenuItem = NSMenuItem()
        regionMenuItem.title = "Region"
        let regionSubMenu = NSMenu()
        let defaults = UserDefaults.standard
        let currentRegion = defaults.value(forKey: "Region") as? Int

        regions.sorted { $0.1 < $1.1 }.forEach { (regionID: Int, value: String) in
            let regionItem = NSMenuItem()
            
            regionItem.title = value
            regionItem.target = self
            regionItem.action = #selector(didTapRegion)
            regionItem.representedObject = regionID
            
            // Toggle as selected
            if currentRegion == regionID {
                regionItem.state = .on
            }
            
            regionSubMenu.addItem(regionItem)
        }
        
        menu.addItem(regionMenuItem)
        menu.setSubmenu(regionSubMenu, for: regionMenuItem)
        
        let reset = NSMenuItem(title: "Reset stats", action: #selector(didTapReset) , keyEquivalent: "r")
        menu.addItem(reset)

        menu.addItem(NSMenuItem.separator())

        menu.addItem(NSMenuItem(title: "Quit", action: #selector(NSApplication.terminate(_:)), keyEquivalent: "q"))

        // 3
        statusItem.menu = menu
    }
    
    private func resetStats() {
        do {
            let path = try FileManager.default.url(
                for: .cachesDirectory,
                in: .userDomainMask,
                appropriateFor: nil,
                create: false
            )
            try FileManager.default.removeItem(atPath: path.path)
        } catch {
            print("ERROR DESCRIPTION: \(error)")
        }
    }
    
    private func getConfigFromIntensity(intensity: String) -> NSImage.SymbolConfiguration {
        switch (intensity) {
            case "very low":
                return NSImage.SymbolConfiguration(pointSize: 22, weight: .ultraLight, scale: .small)
            case "low":
                return NSImage.SymbolConfiguration(pointSize: 22, weight: .thin, scale: .small)
            case "moderate":
                return NSImage.SymbolConfiguration(pointSize: 22, weight: .light, scale: .small)
            case "high":
                return NSImage.SymbolConfiguration(pointSize: 22, weight: .regular, scale: .small)
            case "very high":
                return NSImage.SymbolConfiguration(pointSize: 22, weight: .medium, scale: .small)
            default:
                return NSImage.SymbolConfiguration(pointSize: 22, weight: .light, scale: .small)
        }
    }
    
    func updateCurrentImpact(session: Float, overall: Float, intensity: String, forecast: String) {
        statusItem.menu?.items[0].title = "Session: \(String(format:"%.2f", session)) \(unit)"
        statusItem.menu?.items[1].title = "Lifetime: \(String(format:"%.2f", overall)) \(unit)"
        statusItem.menu?.items[2].title = "Current intensity: \(intensity)"
        statusItem.menu?.items[3].title = forecast

        if let button = statusItem.button {
            var image = NSImage(named: "leif.moods")
            let config = getConfigFromIntensity(intensity: intensity)
            image = image?.withSymbolConfiguration(config)
            button.image = image
        }
    }

    @objc func noop() {}
    
    @objc func didTapRegion(sender: NSMenuItem) {
        guard let regionID = sender.representedObject as AnyObject as? Int else {
            return
        }
        
        let defaults = UserDefaults.standard
        defaults.set(regionID, forKey: "Region")
        
        // Relaunch Leif because I can't figure out how to trigger ContentView to update
        let url = URL(fileURLWithPath: Bundle.main.resourcePath!)
        let path = url.deletingLastPathComponent().deletingLastPathComponent().absoluteString
        let task = Process()
        task.launchPath = "/usr/bin/open"
        task.arguments = [path]
        task.launch()
        exit(0)
    }
    
    @objc func didTapReset() {
        resetStats()
    }
}
