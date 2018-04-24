SRC_DIR="src/"
SRC_FILES="main.c model.c databuffers.c"
FLAGS="-Iinc -g -o modelviewer"
TARGETS=""

for FILE in $SRC_FILES
do
    TARGETS="$TARGETS ${SRC_DIR}${FILE}"
done

echo $TARGETS
gcc -g $FLAGS $TARGETS -lglfw -lGL -lGLU
