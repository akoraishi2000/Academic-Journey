#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Structure for a node in the Huffman tree
struct Node {
    char data;              // Character
    unsigned freq;          // Frequency
    struct Node *left, *right; // Left and right child
};

// Structure for the Min-Heap
struct MinHeap {
    unsigned size;          // Current size of the heap
    unsigned capacity;      // Maximum capacity
    struct Node** array;    // Array of Node pointers
};

// Function to create a new node
struct Node* newNode(char data, unsigned freq) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->left = node->right = NULL;
    node->data = data;
    node->freq = freq;
    return node;
}

// Function to create a min-heap
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct Node**)malloc(minHeap->capacity * sizeof(struct Node*));
    return minHeap;
}

// Function to swap two nodes
void swapNodes(struct Node** a, struct Node** b) {
    struct Node* t = *a;
    *a = *b;
    *b = t;
}

// Min-Heapify function
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Function to check if the min-heap has only one node
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// Function to extract the minimum value node from the heap
struct Node* extractMin(struct MinHeap* minHeap) {
    struct Node* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// Function to insert a new node into the min-heap
void insertMinHeap(struct MinHeap* minHeap, struct Node* node) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

// Function to build the min-heap
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// Function to build the Huffman tree
struct Node* buildHuffmanTree(int freq[]) {
    struct Node *left, *right, *top;
    struct MinHeap* minHeap = createMinHeap(256);

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            insertMinHeap(minHeap, newNode((char)i, freq[i]));
        }
    }

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

// Function to generate Huffman codes
void generateCodes(struct Node* root, int arr[], int top, char* codes[]) {
    if (root->left) {
        arr[top] = 0;
        generateCodes(root->left, arr, top + 1, codes);
    }
    if (root->right) {
        arr[top] = 1;
        generateCodes(root->right, arr, top + 1, codes);
    }
    if (!root->left && !root->right) {
        codes[(unsigned char)root->data] = (char*)malloc((top + 1) * sizeof(char));
        for (int i = 0; i < top; i++) {
            codes[(unsigned char)root->data][i] = arr[i] + '0';
        }
        codes[(unsigned char)root->data][top] = '\0';
    }
}

// Function to compress the file
void compressFile(FILE* input, FILE* output, char* codes[]) {
    char ch;
    unsigned char buffer = 0;
    int bitCount = 0;

    while ((ch = fgetc(input)) != EOF) {
        for (int i = 0; codes[(unsigned char)ch][i] != '\0'; i++) {
            buffer <<= 1;
            if (codes[(unsigned char)ch][i] == '1') {
                buffer |= 1;
            }
            bitCount++;
            if (bitCount == 8) {
                fwrite(&buffer, 1, 1, output);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // Write remaining bits
    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        fwrite(&buffer, 1, 1, output);
    }
}

// Function to decompress the file
void decompressFile(FILE* input, FILE* output, struct Node* root) {
    struct Node* current = root;
    unsigned char buffer;
    int bitCount = 0;

    while (fread(&buffer, 1, 1, input)) {
        for (int i = 7; i >= 0; i--) {
            int bit = (buffer >> i) & 1;
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }

            if (!current->left && !current->right) {
                fputc(current->data, output);
                current = root;
            }
        }
    }
}

// Main function
int main() {
    FILE *input, *compressed, *decompressed;
    input = fopen("input.txt", "r");
    compressed = fopen("compressed.bin", "wb");
    decompressed = fopen("decompressed.txt", "w");

    if (!input || !compressed || !decompressed) {
        printf("Error opening files!\n");
        return 1;
    }

    // Step 1: Calculate character frequencies
    int freq[256] = {0};
    char ch;
    while ((ch = fgetc(input)) != EOF) {
        freq[(unsigned char)ch]++;
    }
    rewind(input);

    // Step 2: Build the Huffman tree
    struct Node* root = buildHuffmanTree(freq);

    // Step 3: Generate Huffman codes
    char* codes[256] = {NULL};
    int arr[100], top = 0;
    generateCodes(root, arr, top, codes);

    // Step 4: Compress the file
    compressFile(input, compressed, codes);

    // Step 5: Decompress the file
    fclose(compressed);
    compressed = fopen("compressed.bin", "rb");
    decompressFile(compressed, decompressed, root);

    // Cleanup
    fclose(input);
    fclose(compressed);
    fclose(decompressed);

    printf("File compressed and decompressed successfully!\n");
    return 0;
}
