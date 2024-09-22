# TradeZest

TradeZest is a basic trading application developed in C++ as a study case. It is designed to practice programming techniques and explore the use of modern C++ features but is **not intended for actual trading purposes**. TradeZest integrates with the Free (OANDA API)[https://www.oanda.com/uk-en/trading/] to simulate the creation of orders and trades.

## Features

- **Trade Simulation**: Create and manage simulated trades using the Free OANDA API.
- **Modern C++ (C++20)**: Leverages C++20 features for enhanced code readability and performance.
- **Dependency Injection**: Enhances testability and modularity using Dependency Injection principles.
- **Unit Testing**: Uses Google Test and Google Mock frameworks for comprehensive unit testing.
- **Cross-Platform Build System**: Utilizes CMake for easy building and management of the project.
- **Boost and STL**: Incorporates Boost libraries and the Standard Template Library (STL) for efficient data structures and algorithms.

## Getting Started

### Prerequisites

To build and run TradeZest, you will need:

- **C++20 compatible compiler**
- **CMake** (version 3.20 or higher)
- **Boost** (version 1.86 or higher)
- **Google Test** (automatically fetched via CMake if not present)

### Setup

1. **Clone the repository**:
   ```bash
   git clone https://github.com/arielberardi/TradeZest.git
   cd TradeZest
   ```

2. **Configure the project with CMake**:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. **Build the project**:
   ```bash
   cmake --build .
   ```

4. **Run Tests**:
   TradeZest uses Google Test for unit testing. To run the tests:
   ```bash
   ctest
   ```

### Running the Application

Once built, you can run the application from the `build` directory:

```bash
./TradeZest
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

## Disclaimer

**TradeZest is intended for educational and practice purposes only. It is not a professional trading application and should not be used for real trading.**

## Contact

For any questions or suggestions, please reach out [Ariel Berardi](https://www.linkedin.com/in/aberardi95/).
