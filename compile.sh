export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH

#---------------------------------------------------------------------#

if [ ! -d "build" ]; then
    mkdir "build"
fi

#---------------------------------------------------------------------#

echo -e "compilling ungine server"
if [ ! -f  "./build/server" ] ; then

    g++ -o ./build/server ./service/server.cpp -lssl -lcrypto -lz -I./include/backend

    if [ ! $? -eq 0 ]; then
        echo "exit error"; exit;
    fi

fi

#---------------------------------------------------------------------#

echo -e "compilling ungine game" ; rm ./view/index.html
if [ ! -f  "./view/index.html" ] ; then

    em++ -o ./view/index.html ./service/game.cpp -lraylib \
    -I./include/frontend -L./lib -lwebsocket.js -pthread \
    -s NO_DISABLE_EXCEPTION_CATCHING --bind -lembind \
    -s FETCH=1 -s WASM=1 -s EXPORT_NAME="'Engine'" \
    -s USE_GLFW=3 -s ASYNCIFY=1 -s ASSERTIONS=0 \
    -s PTHREAD_POOL_SIZE=8 -s USE_PTHREADS=1 \
    --embed-file ./controller/assets \
    -s MODULARIZE=1 --shell-file ./view/shell.html

    if [ ! $? -eq 0 ]; then
        echo "exit error"; exit;
    fi

fi

#---------------------------------------------------------------------#

echo -e "running ungine" ; ./build/server
