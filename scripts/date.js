function dateCallback(dateOutput) {
	bot.sendPrivateMessage(dateOutput);
}

function execute() {
	bot.execute("date", dateCallback);
}

