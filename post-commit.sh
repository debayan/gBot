#!/bin/sh
#
# An example hook script that is called after a successful
# commit is made.
#
# To enable this hook, rename this file to "post-commit".

str1=`cd ../../;pwd``git log -n 1`
echo $str1

#qdbus com.forwardbias.git.commit / local.Bot.commit "$str1"
