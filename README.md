# Requirements
- CMake needs to be installed
- Check the top of the game_server.py file to correct the platform
- Not tested on Linux


# Steps
1. Create env

```
python -m venv .venv
```

2. Activate env
Windows (git bash)
```
source .venv\Scripts\Activate
```
Mac
```
source .venv/bin/activate
```

3. Install requirements
```
pip install -r requirements.txt
```

4. Compile (requires cmake)
```
chmod +x build_project.sh
./build_project.sh
```
If you get an error, follow the steps in the script manually

5. Run (root folder)
```
python game_server.py
```

# Frequent Problems
The first boot, it's best to have a game running, with an empty board (pieces clip the top and right side of the board)
and wait for "Board Setup Complete" message.
- Import "build2.agents" could not be resolved: Windows creates an extra folder, change to build2.Release.agents
- Agent playing weird: Make sure the current piece isn't stuck inside the top of the board while it scans at start
- Agent not playing at all: Piece colors might be off, unfortuantely the fix is getting the values using a color picker and updating the code in game_server.py
- Agent not playing at all: Input mapping might be wrong, check the input mapping in the game_server.py
