CLIENT := client
SERVER := server
SOURCE_CLIENT := client.c
SOURCE_SERVER := server.c
CC := gcc
CFLAGS := 

$(CLIENT):
	clear && $(CC) $(SOURCE_CLIENT) $(CFLAGS) -o$(CLIENT)
$(SERVER):
	clear && $(CC) $(SOURCE_SERVER) $(CFLAGS) -o$(SERVER)
clean:
	rm -rf $(CLIENT) $(SERVER)