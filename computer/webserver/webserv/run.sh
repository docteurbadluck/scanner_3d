#!/bin/bash

WEBSERV_DIR="$(cd "$(dirname "$0")" && pwd)"
OLD_PREFIX="/home/docteurbadluck/Desktop/3D_scanner"
TMP_CONF="/tmp/webserv_local.conf"

resolve_project_root() {
    cd "$WEBSERV_DIR/../../.." && pwd
}

generate_local_conf() {
    local project_root="$1"
    sed "s|$OLD_PREFIX|$project_root|g" "$WEBSERV_DIR/config.conf" > "$TMP_CONF"
    mkdir -p "$project_root/computer/webserver/www/upload"
}

main() {
    local project_root
    project_root="$(resolve_project_root)"
    generate_local_conf "$project_root"
    exec "$WEBSERV_DIR/webserv" "$TMP_CONF"
}

main "$@"
