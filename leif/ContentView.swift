//
//  ContentView.swift
//  leif
//
//  Created by Tim Stone on 05/07/2022.
//

import SwiftUI

struct ContentView: View {
    
    @EnvironmentObject var appDelegate: AppDelegate
    @StateObject private var vm: LeifViewModel
    @State private var currentWatts: Float = 0
    
    init(vm: LeifViewModel) {
        self._vm = StateObject(wrappedValue: vm)
    }
    
    func updateWatts() {
        var battery = Battery()
        battery.open()
        let isCharging = battery.isACPowered()
        
        // If charging we want to start or continue to collect watt usage
        if (isCharging) {
            var payload = vm.createOrFetchPreviousWatts()
            let watts = battery.getPowerDrawInWatts()
            payload = vm.addNewMeasurement(payload: payload, watts: watts)
            try? vm.storage!.save(payload, for: "watts")
            
            // Update the menu bar
            appDelegate.updateCurrentImpact(
                session: payload.total_session,
                overall: payload.total_overall
            )
            Task {
                await vm.populateImpact()
            }
        } else {
            let payload = vm.removePreviousWatts()
            appDelegate.updateCurrentImpact(
                session: payload.total_session,
                overall: payload.total_overall
            )
        }
        battery.close()
    }
    
    var body: some View {
        ScrollView {}.task {
            updateWatts()
        }.onReceive(vm.timer) { time in
            updateWatts()
       }.frame(width: .zero)
    }
}
