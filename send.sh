#!/bin/bash
#set url and port to the xbmc box webservice
XBMC_HOST="http://192.168.0.4:8080"

if [ "$1" = "" ]; then
 echo -n "Insert URL: "
 read url
else
 url="$1"
fi

if [[ "$url" == *youtube.com* ]]
then
  vid=$( echo "$url" | tr '?&#' '\n\n' | grep -e '^v=' | cut -d'=' -f2 )
  payload='{"jsonrpc": "2.0", "method": "Player.Open", "params":{"item": {"file" : "plugin://plugin.video.youtube/?action=play_video&videoid='$vid'" }}, "id" : "1"}'
elif [[ "$url" == *vimeo.com* ]]
then
  vid=$( echo "$url" | awk -F"/" '{print ($(NF))}' )
  payload='{"jsonrpc": "2.0", "method": "Player.Open", "params":{"item": {"file" : "plugin://plugin.video.vimeo/?action=play_video&videoid='$vid'" }}, "id": "1" }'
else
  payload='{ "jsonrpc": "2.0", "method": "Player.Open", "params": { "item": { "file": "'${url}'" }}, "id": 1 }'
fi

curl -v -u xbmc:password -d "$payload" -H "Content-type:application/json" -X POST "${XBMC_HOST}/jsonrpc"
