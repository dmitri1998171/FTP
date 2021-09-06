CLIENT := client
SERVER := server
SOURCE_CLIENT := client.c commands.c
SOURCE_SERVER := server.c
SOURCE_EXT := extentions.c
CC := gcc
CFLAGS := -g

$(CLIENT):
	clear && $(CC) $(SOURCE_CLIENT) $(SOURCE_EXT) $(CFLAGS) -o$(CLIENT)
$(SERVER):
	clear && $(CC) $(SOURCE_SERVER) $(SOURCE_EXT) $(CFLAGS) -o$(SERVER)
clean:
	rm -rf $(CLIENT) $(SERVER)