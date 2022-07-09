//
//  IntensityViewModel.swift
//  leif
//
//  Created by Tim Stone on 09/07/2022.
//

import Foundation

struct IntensityViewModel {
    
    private var intensity: CO2_Data
    
    init(intensity: CO2_Data) {
        self.intensity = intensity
    }
    
    var data: Intensity {
        intensity.intensity
    }
    
    var id: Int {
        Int.random(in: 1..<5)
    }
}
