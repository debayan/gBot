function processCallback(output) {
    var out = output.split('\n');
    for (var i = 0; i < out.length; i++)
        bot.sendPrivateMessage(out[i]);
}

function execute( argument ) {
    bot.execute("fortune "+argument, processCallback);
}

