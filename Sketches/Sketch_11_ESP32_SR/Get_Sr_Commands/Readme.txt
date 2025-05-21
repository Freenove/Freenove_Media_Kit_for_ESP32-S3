1. Install the gen_en library: python install_gen_en.py

2. Run the code to generate speech recognition instructions.
	python gen_sr_commands.py "Turn on the light,Switch on the light;Turn off the light,Switch off the light,Go dark;Start fan;Stop fan"

	Replace the generated content with the corresponding area of the code.
	enum {
	  SR_CMD_TURN_ON_THE_LIGHT,
	  SR_CMD_TURN_OFF_THE_LIGHT,
	  SR_CMD_START_FAN,
	  SR_CMD_STOP_FAN,
	};
	static const sr_cmd_t sr_commands[] = {
	  { 0, "Turn on the light", "TkN nN jc LiT"},
	  { 0, "Switch on the light", "SWgp nN jc LiT"},
	  { 1, "Turn off the light", "TkN eF jc LiT"},
	  { 1, "Switch off the light", "SWgp eF jc LiT"},
	  { 1, "Go dark", "Gb DnRK"},
	  { 2, "Start fan", "STnRT FaN"},
	  { 3, "Stop fan", "STnP FaN"},
	};