#include "stock_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>

#define API_URL "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=%s&apikey=NA06S3F7Z0KFS3EQ"

struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback Function for Curl
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
    char *ptr = realloc(mem->memory, mem->size + total_size + 1);
    if (!ptr) return 0;
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, total_size);
    mem->size += total_size;
    mem->memory[mem->size] = 0;
    return total_size;
}

// Fetch Live Stock Price
double get_live_stock_price(const char *symbol) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk = {malloc(1), 0};
    double price = -1.0;
    char url[256];
    snprintf(url, sizeof(url), API_URL, symbol);
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl\n");
        return price;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
    res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        json_t *root, *quote, *price_json;
        json_error_t error;
        root = json_loads(chunk.memory, 0, &error);
        if (root) {
            quote = json_object_get(root, "Global Quote");
            if (quote) {
                price_json = json_object_get(quote, "05. price");
                if (json_is_string(price_json)) {
                    price = atof(json_string_value(price_json));
                }
            }
            json_decref(root);
        }
    } else {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    free(chunk.memory);
    curl_easy_cleanup(curl);
    return price;
}
