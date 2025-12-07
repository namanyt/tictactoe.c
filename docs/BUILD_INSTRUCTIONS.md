# Build Instructions

## Prerequisites

Before building this project, ensure you have the following installed:

- **GCC** (GNU Compiler Collection) - C compiler
- **Make** - Build automation tool
- **Standard C Library** - Usually comes with your system

### Checking Prerequisites

```bash
# Check GCC version
gcc --version

# Check Make version
make --version
```

## Building from Source

### Step 1: Navigate to Project Directory

```bash
cd path/to/tictactoe.c
```

### Step 2: Compile Using Make

```bash
make
```

This command will:
- Compile all `.c` source files (main.c, game.c, ai.c, utils.c, ui.c)
- Use GCC with compiler flags:
  - `-Wall` - Enable all common compiler warnings
  - `-Wextra` - Enable extra warnings
  - `-std=c2x` - Use C23 standard (latest C standard)
- Link all object files together
- Create the executable: `tictactoe`

**Expected output:**
```
gcc -Wall -Wextra -std=c2x -o tictactoe main.c game.c ai.c utils.c ui.c
```

### Step 3: Verify Build Success

The build was successful if:
1. No error messages appear
2. A `tictactoe` executable file is created in the current directory
3. The file is executable (shows `x` permissions: `ls -l tictactoe`)

### Step 4: Run the Program

```bash
./tictactoe
```

## Makefile Details

The `Makefile` contains the following targets:

### `make` or `make all` (default)
Compiles the entire project and creates the executable.

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c2x
SRCS = main.c game.c ai.c utils.c ui.c
OBJS = $(SRCS:.c=.o)
TARGET = tictactoe

all: $(TARGET)

$(TARGET): $(SRCS)
$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)
```

### `make clean`
Removes all build artifacts:
- Deletes the compiled executable (`tictactoe`)
- Deletes all object files (`.o` files)

```bash
make clean
```

**Note:** Does NOT delete runtime files (game_stats.txt, leaderboard.txt) - those are preserved to maintain player records.

## Compiler Flags Explained

| Flag | Purpose |
|------|---------|
| `-Wall` | Enable all standard compiler warnings (helps catch common bugs) |
| `-Wextra` | Enable additional warnings beyond `-Wall` for stricter checking |
| `-std=c2x` | Use C23 standard (latest C standard with modern features) |
| `-o` | Specify output filename for the executable |

## Troubleshooting

### "make: command not found"
Install Make:
- **macOS:** `brew install make` or use Xcode Command Line Tools
- **Linux (Debian/Ubuntu):** `sudo apt-get install build-essential`
- **Linux (Fedora/RHEL):** `sudo dnf install make gcc`
- **Windows:** Use MinGW, Cygwin, or WSL (Windows Subsystem for Linux)

### "gcc: command not found"
Install GCC:
- **macOS:** `xcode-select --install`
- **Linux (Debian/Ubuntu):** `sudo apt-get install build-essential`
- **Linux (Fedora/RHEL):** `sudo dnf install gcc`
- **Windows:** Download MinGW or use WSL

### Compilation Errors
1. Check that all source files (.c files) are in the same directory as Makefile
2. Verify header files (.h files) have matching names
3. Check for syntax errors in the code
4. Ensure you're using the correct version of the C standard (c2x)

### Permission Denied When Running
After building, make the executable runnable:
```bash
chmod +x tictactoe
./tictactoe
```

## Building with Custom Flags (Advanced)

To compile with additional debugging information:

```bash
gcc -Wall -Wextra -std=c2x -g -o tictactoe main.c game.c ai.c utils.c ui.c
```

The `-g` flag adds debugging symbols for use with GDB debugger.

## Clean Build

To perform a clean build (remove old files, then recompile):

```bash
make clean && make
```

This ensures all files are freshly compiled without any cached object files.
