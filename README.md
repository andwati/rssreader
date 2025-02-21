# Modern RSS Reader

A modern, feature-rich RSS feed reader built with C++17 and Qt6. This application allows you to manage, read, and organize your RSS feeds with a clean and efficient interface.

## Features

- Multi-panel interface for easy feed navigation
- Category management for feed organization
- Customizable feed formatting and display options
- Persistent storage of feeds and settings
- Auto-refresh functionality
- Feed management (add, delete, reorder)
- Article read status tracking
- Rich text content rendering

## Prerequisites

- C++17 compatible compiler
    - GCC 7+ (Linux)
    - MSVC 2019+ (Windows)
    - Clang 5+ (macOS)
- CMake 3.16 or higher
- Qt 6.x
- Git (for cloning the repository)

## Building from Source

### Linux

1. Install dependencies:
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-tools-dev

# Fedora
sudo dnf install gcc-c++ cmake qt6-qtbase-devel qt6-tools
```

2. Clone the repository:
```bash
git clone https://github.com/yourusername/rss-reader.git
cd rss-reader
```

3. Create build directory and build:
```bash
mkdir build
cd build
cmake ..
cmake --build . -j$(nproc)
```

### Windows

1. Install prerequisites:
    - [Visual Studio 2019 or later](https://visualstudio.microsoft.com/downloads/)
    - [Qt 6.x](https://www.qt.io/download)
    - [CMake](https://cmake.org/download/)

2. Clone the repository:
```cmd
git clone https://github.com/yourusername/rss-reader.git
cd rss-reader
```

3. Configure and build:
```cmd
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=C:\Qt\6.x\msvc2019_64
cmake --build . --config Release
```

Note: Adjust the Qt path according to your installation.

### macOS

1. Install prerequisites using Homebrew:
```bash
brew install cmake qt@6
```

2. Clone the repository:
```bash
git clone https://github.com/yourusername/rss-reader.git
cd rss-reader
```

3. Build the project:
```bash
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
cmake --build . -j$(sysctl -n hw.ncpu)
```

## Installation

### Linux
```bash
cd build
sudo cmake --install .
```

The application will be installed to `/usr/local/bin` by default.

### Windows
```cmd
cd build
cmake --install . --config Release
```

### macOS
```bash
cd build
cmake --install .
```

## Running

After installation, you can run the RSS reader:

- Linux: Run `rss_reader` from terminal or launch from application menu
- Windows: Run from Start Menu or `rss_reader.exe` in the installation directory
- macOS: Open from Applications folder or run `rss_reader` from terminal

## Development

### Project Structure
```
rss_reader/
├── CMakeLists.txt          
├── include/                
│   ├── feed.hpp
│   ├── feed_parser.hpp
│   ├── main_window.hpp
│   ├── settings_dialog.hpp
│   └── storage.hpp
├── src/                    
│   ├── main.cpp
│   ├── main_window.cpp
│   ├── feed_parser.cpp
│   ├── settings_dialog.cpp
│   └── storage.cpp
└── resources/             
    ├── icons/
    ├── style/
    └── resources.qrc
```

### Building for Development

For development builds with debug symbols:

```bash
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

### Running Tests (if implemented)

```bash
cd build
ctest --output-on-failure
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.

## Support

For bug reports and feature requests, please use the GitHub issue tracker.

# TODO
- [ ] Cache feed content locally using SQLite or similar
- [ ] Allow reading cached content when offline
- [ ] Implement background sync for feeds
- [ ] Store images locally for offline viewing
- [ ] Full-text search across all feeds
- [ ] Advanced filtering (date ranges, read/unread, keywords)
- [ ] Tag system for articles
- [ ] Smart categorization using NLP/keywords
- [ ] OPML file import/export for feed lists
- [ ] Export articles to PDF/HTML/Markdown
- [ ] Backup/restore of all settings and data
- [ ] Browser bookmark import
- [ ] Keyboard shortcuts for navigation
- [ ] Reading progress tracking
- [ ] Article preview on hover
- [ ] Infinite scroll for articles
- [ ] Dark mode support
- [ ] Multi-language UI
- [ ] Reading time estimates
- [ ] Update frequency analysis
- [ ] Most active feeds tracking
- [ ] Content similarity detection
- [ ] Reading habits dashboard
- [ ] Share articles to social media
- [ ] Save articles to read-later services (Pocket, Instapaper)
- [ ] Comment system for personal notes
- [ ] Feed recommendations based on reading habits
- [ ] Feed health monitoring (detect dead feeds)
- [ ] Duplicate article detection
- [ ] Feed merge/split capabilities
- [ ] Feed content rules (filters, transformations)
- [ ] Feed content sanitization
- [ ] Secure storage of credentials
- [ ] Content verification (checksums)
- [ ] SSL certificate validation