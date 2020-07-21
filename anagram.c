#include <stdio.h>
#include <stdlib.h>

struct Node {
  int end_word;
  struct Node* next[26];
};

struct Tile {
  char letter;
  int amount;
};

struct Node* get_new_node(){
  struct Node* node = malloc(sizeof(struct Node));
  node->end_word = 0;

  for(int i = 0; i < 26; i++)
    node->next[i] = NULL;

  return node;
}

void insert(struct Node* head, char* word){
  struct Node* node = head;
  while (*word != '\r'){
    if(node->next[*word - 'A'] == NULL)
      node->next[*word - 'A'] = get_new_node();

    node = node->next[*word - 'A'];
    word++;
  }
  node->end_word = 1;
}

void load_words(char* filename, struct Node* head){
  FILE* wordlist = fopen(filename, "r");
  while(1){
    char* word = malloc(sizeof(char) * 60);
    if(fgets(word, 60, wordlist) == NULL) break;
    insert(head, word);
  }
}

void add_tile_to_buffer(struct Tile* tile, char* end_of_buffer, char current_letter){
  tile->amount--;
  *end_of_buffer = current_letter;
}

void remove_tile_from_buffer(struct Tile* tile, char* buffer){
  tile->amount++;
  *buffer = 0;
}

void get_anagrams(struct Node* root, struct Tile tiles[], char* buffer, int buffer_pointer);

void traverse_trie(struct Node* root, struct Tile tiles[], int tile_pointer, char* buffer, int buffer_pointer, char letter){
  struct Node* current_node = root->next[letter - 'A'];
  if(current_node != NULL){
    add_tile_to_buffer(tiles + tile_pointer, buffer + buffer_pointer, letter);
    if(current_node->end_word) printf("%s\n", buffer);
    get_anagrams(current_node, tiles, buffer, buffer_pointer + 1);
    remove_tile_from_buffer(tiles + tile_pointer, buffer + buffer_pointer);
  }
}

void get_anagrams(struct Node* root, struct Tile tiles[], char* buffer, int buffer_pointer){
  struct Node* current_node;
  struct Tile* tile;
  char current_letter;
  char start_letter;
  char end_letter;
  for(int tile_pointer = 0; tile_pointer < 27; tile_pointer++){
    current_letter = tiles[tile_pointer].letter;
    if(current_letter != '?'){
      start_letter = current_letter;
      end_letter = current_letter;
    }
    else{
      start_letter = 'A';
      end_letter = 'Z';
    }
    for(current_letter = start_letter; current_letter <= end_letter; current_letter++){
      if(tiles[tile_pointer].amount > 0)
        traverse_trie(root, tiles, tile_pointer, buffer, buffer_pointer, current_letter);
    }
  }
}

int main(int argc, char *argv[]){
  if(argc != 2)
    printf("One argument expected.\n");
  else{
    int tile_amount[27] = {0};
    int size = 0;
    int unique_tiles = 0;
    int tile_index = 0;
    char* letters = argv[1];
    struct Tile tiles[27];
    for(int i = 0; i < 27; i++)
      tiles[i].amount = 0;
    while(*letters != 0) {
      if(*letters >= 'a' && *letters <= 'z'){
        *letters -= 32;
        tile_index = *letters - 'A';
      }
      else if(*letters >= 'A' && *letters <= 'Z')
        tile_index = *letters - 'A';
      else if(*letters == '?')
        tile_index = 26;
      else
        tile_index = 27;
      if(tile_index != 27){
        tiles[tile_index].letter = *letters;
        tiles[tile_index].amount++;
      }
      letters++;
    }
    char* buffer = malloc(sizeof(char) * 27);
    struct Node* root = malloc(sizeof(struct Node));
    load_words("sowpods.txt", head);
    get_anagrams(root, tiles, buffer, 0);
  }
  return 0;
}
