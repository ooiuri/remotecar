# build step
FROM node:20-alpine3.17 as build

ENV NODE_OPTIONS --openssl-legacy-provider

# ENV USER_NAME=front
# ENV GROUP_ID=1000
# ENV USER_ID=1000

# RUN addgroup -g $GROUP_ID ${USER_NAME} \
#     && adduser -D -u $USER_ID -G ${USER_NAME} ${USER_NAME} -s /bin/s

# RUN chown -R ${USER_NAME}:${USER_NAME} /app

# USER ${USER_NAME}

WORKDIR /app
COPY package.json ./
RUN yarn
COPY . ./
CMD ["yarn", "dev"] 

# # release step
# FROM nginx:1.21.5-alpine as release
# COPY --from=build /app/build /usr/share/nginx/html/
# EXPOSE 80
# CMD ["nginx", "-g", "daemon off;"]
