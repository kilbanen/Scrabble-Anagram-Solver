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

void get_anagrams(struct Node* trie_head, struct Tile letters[], int size, char buffer[], int buffer_pointer){
  struct Node* current_node;
  for(int i = 0; i < size; i++){
    if(trie_head->next[letters[i].letter - 'A'] != NULL && letters[i].amount > 0){
      current_node = trie_head->next[letters[i].letter - 'A'];
      letters[i].amount--;
      buffer[buffer_pointer] = letters[i].letter;
      if(current_node->end_word) printf("%s\n", buffer);
      get_anagrams(current_node, letters, size, buffer, buffer_pointer + 1);
      letters[i].amount++;
      buffer[buffer_pointer] = 0;
    }
  }
}

int main(int argc, char *argv[]){
  if(argc != 2)
    printf("One argument expected.\n");
  else{
    int tile_amount[26] = {0};
    int size = 0;
    int unique_tiles = 0;
    char* letters = argv[1];
    while(*letters != 0) {
        if(*letters >= 'a' && *letters <= 'z')
          *letters -= 32;
        if(*letters >= 'A' && *letters <= 'Z'){
        if(tile_amount[*letters - 'A'] == 0)
          unique_tiles++;
        tile_amount[*letters - 'A']++;
        size++;
      }
      letters++;
    }
    struct Tile tiles[unique_tiles];
    char buffer[size];
    int tile_index = 0;
    for(int i = 0; i < 26; i++){
      if(tile_amount[i] > 0){
        tiles[tile_index].letter = i + 'A';
        tiles[tile_index].amount = tile_amount[i];
        tile_index++;
      }
    }
    struct Node* head = malloc(sizeof(struct Node));
    load_words("sowpods.txt", head);
    get_anagrams(head, tiles, unique_tiles, buffer, 0);
  }
  return 0;
}
