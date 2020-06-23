#include <stdio.h>
#include <stdlib.h>

struct Node {
  int end_word;
  struct Node* next[26];
};

struct Tile {
  char letter;
  int played;
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
    if(trie_head->next[letters[i].letter - 'A'] != NULL && letters[i].played != 1){
      current_node = trie_head->next[letters[i].letter - 'A'];
      letters[i].played = 1;
      buffer[buffer_pointer] = letters[i].letter;
      if(current_node->end_word) printf("%s\n", buffer);
      get_anagrams(current_node, letters, size, buffer, buffer_pointer + 1);
      letters[i].played = 0;
      buffer[buffer_pointer] = 0;
    }
  }
}

int main(int argc, char *argv[]){
  if(argc != 2)
    printf("One argument expected.\n");
  else{
    int size = 0;
    char* letters = argv[1];
    while(*letters != 0) {
      size++;
      letters++;
    }
    letters -= size;
    struct Tile tiles[size];
    char buffer[size];
    for(int i = 0; i < size; i++){
      tiles[i].letter = letters[i];
      tiles[i].played = 0;
    }
    struct Node* head = malloc(sizeof(struct Node));
    load_words("sowpods.txt", head);
    get_anagrams(head, tiles, size, buffer, 0);
  }
  return 0;
}
