Based on configuration from https://github.com/SasLuca/raylib-cmake-template

<a href="https://www.youtube.com/watch?v=NyV3he_dnt0">
  <img width="864" alt="image" src="https://github.com/user-attachments/assets/006b2e0e-828f-4648-81ed-6d443bf4c4b1" />
</a>

## Description

The cells can "evolve" through mutations, which occur at a 30% rate in this version.
I added a simple perceptron model to determine cell replication behaviour. The model has three inputs: age, health, and the nutrient levels beneath the cell.

This project taught me much about Raylib and C++; I’m ready to move on.

Some ideas I'd like to implement if I have time:
- All cell behaviour goes from the brain, nothing is hardcoded
- Parallel on multiple cores or even GPU to enable bigger simulations
- Add more environmental factors like sun, water, temperature
- Add more views to gather more data. Add charts with population rates etc
- Add brushes to draw on the environment
- Add multiplayer where users grow their species and then clash them


## Build and run

Was tested on MacOS

```sh
cd build
cmake .. # builds using cmake with specs from CMakeLists.txt and puts the output in build folder
make # compiles code
./artificial-life
```
