#  leif

leif is an open-source macOS menu bar app to track your computer's CO2 equivalent emissions over time.

It does this by counting the average power consumption while your MacBook is charging. Data is saved locally and can be reset at any time.

It can also use this data to tell you when to charge in the near future to minimise your carbon impact.

CO2 data is provided by the National Grid via https://carbonintensity.org.uk.

I tried to use permacomputing principles while developing this by:

* going native, this [started as a Ruby script](https://gist.github.com/fetimo/bfddb1d35524106357e922cd64fcaa2a) but in a bid to minimise CPU and memory usage while running it I opted to use Swift instead
* making it clear when your computer is using energy contributing to climate change
* minimised dependencies, no libraries other than those provided by macOS

## Requirements and limitations

* UK only (excluding Northern Ireland)
* macOS 12+
* only tested on MacBook Pros

## Work remaining

I need to refactor the code to work with older macOS versions (and see how feasible it is).
