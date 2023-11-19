FROM ailispaw/barge:armhf

RUN pkg install mjpg-streamer

EXPOSE 8090

ENTRYPOINT [ "dumb-init", "--", "mjpg_streamer" ]

CMD [ "-i", "input_uvc.so -y -d /dev/video1 -r 320x240 -f 10", "-o", "output_http.so -w /usr/www -p 8090" ]