CLIENT := client
SERVER := server
SOURCE_CLIENT := client.c
SOURCE_SERVER := server.c
SOURCE_EXT := commands.c extentions.c
CC := gcc
CFLAGS := 

$(CLIENT):
	clear && $(CC) $(SOURCE_CLIENT) $(SOURCE_EXT) $(CFLAGS) -o$(CLIENT)
$(SERVER):
	clear && $(CC) $(SOURCE_SERVER) $(SOURCE_EXT) $(CFLAGS) -o$(SERVER)
clean:
	rm -rf $(CLIENT) $(SERVER)