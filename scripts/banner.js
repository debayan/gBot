function figletCallback(dateOutput) {
	bot.sendPrivateMessage(dateOutput);
}

function execute() {
	bot.execute("figlet ForwardBias", figletCallback);
}

