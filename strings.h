/*
 *  Copyright © 2025 hadron13
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the “Software”), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef STRINGS_H
#define STRINGS_H

typedef struct{
    char  *cstr;
    ptrdiff_t length;
}string_t;

#ifndef PATH_SEPARATOR
#   define PATH_SEPARATOR '/'
#   define PATH_SEPARATOR_STR "/"
#endif 

#define str(cstring) ((string_t){.length = strlen(cstring), .cstr = (cstring)})
#define str_null     ((string_t){.length = 0, .cstr = NULL})
#define str_alloc(str_size) ((string_t){.length = (str_size), .cstr = malloc((str_size) + 1)})
#define str_free(string) (free((string).cstr))

#define string_join(...)  (string_join_(str(""),__VA_ARGS__, str_null))
#define string_path(...)  (string_join_(str(PATH_SEPARATOR_STR),__VA_ARGS__, str_null))
#define string_join_sep(separator, ...)  (string_join_((separator),__VA_ARGS__, str_null))

static string_t string_join_(string_t separator, ...);
static string_t string_dup(string_t string);
static string_t string_dup_len(string_t string, size_t length);
static bool     string_equal(string_t a, string_t b);
static string_t string_get_ext(string_t filename);
static string_t string_get_path(string_t filename);

typedef string_t* string_temp_t;

static string_t str_temp(string_temp_t *temp, string_t string);
static void     str_temp_free(string_temp_t temp);
#define  str_temp_join(temp, ...) (str_temp(temp, string_join_(str(""),__VA_ARGS__, str_null)))



static string_t string_join_(string_t separator, ...){
    va_list args;
    va_start(args, separator);
    
    size_t total_size = 0;
    string_t current_string = va_arg(args, string_t);

    while(current_string.cstr != NULL){
        total_size += current_string.length + separator.length; 
        current_string = va_arg(args, string_t);
    }
    total_size -= separator.length;
    va_end(args);

    string_t joined_string = str_alloc(total_size);
    va_start(args, separator);
    
    for(size_t current_character = 0; current_character < total_size;){
        if(current_character != 0){
            memcpy(joined_string.cstr + current_character, separator.cstr, separator.length);
            current_character += separator.length;
        }
        string_t current_string = va_arg(args, string_t);

        memcpy(joined_string.cstr + current_character, current_string.cstr, current_string.length);
        current_character += current_string.length;
    }
    joined_string.cstr[total_size] = '\0';
    va_end(args);
    return joined_string;
}

static string_t string_dup(string_t string){
    string_t duplicated = str_alloc(string.length);
    memcpy(duplicated.cstr, string.cstr, string.length);
    duplicated.cstr[string.length] = '\0';
    return duplicated;
}

static string_t string_dup_len(string_t string, size_t length){
    string_t duplicated = str_alloc(string.length);
    memcpy(duplicated.cstr, string.cstr, length);
    duplicated.cstr[length] = '\0';
    return duplicated;
}

static bool string_equal(string_t a, string_t b){
    return a.length == b.length && memcmp(a.cstr, b.cstr, a.length) == 0;
}


static string_t string_get_ext(string_t filename){
    if(!filename.cstr)
        return str_null;
    
    char *last_point = filename.cstr + filename.length - 1;
    while( *last_point != '.' && last_point != filename.cstr)
        last_point--;

    if(last_point == filename.cstr)
        return filename;
    return string_dup((string_t){.cstr = last_point, .length = last_point - filename.cstr});
}

static string_t string_get_filename_no_ext(string_t filename){
    
    char *last_slash = filename.cstr + filename.length;
    while( *last_slash != PATH_SEPARATOR && last_slash != filename.cstr)
        last_slash--;

    last_slash += 1;

    char *filename_dot = last_slash;
    while( *filename_dot != '.' && filename_dot != filename.cstr + filename.length - 1)
        last_slash++;

    return string_dup_len(
        (string_t){
            .cstr = last_slash,
            .length = filename_dot - last_slash,
        },
        filename_dot - last_slash
    );

}

static string_t string_get_path(string_t filename){
    if(!filename.cstr)
        return str_null;
    
    char *last_slash = filename.cstr + filename.length;
    while( *last_slash != PATH_SEPARATOR && last_slash != filename.cstr)
        last_slash--;
#   ifdef WINDOWS
    char *alt_slash = filename.cstr + filename.length;
    while( *alt_slash != '/'&& last_slash != filename.cstr)
        alt_slash--;

    if(alt_slash && alt_slash > last_slash){
        last_slash = alt_slash;
    }
#   endif
    
    if(!last_slash)
        return filename;
    return string_dup_len((string_t){
        .cstr = filename.cstr,
        .length = last_slash - filename.cstr
    }, last_slash - filename.cstr);
}

        

static string_t str_temp(string_temp_t *arena, string_t str){
    list_push(*arena, str);
    return str;
}

static void str_temp_free(string_temp_t temp){
    for(size_t i = 0; i < list_size(temp); i++){
        str_free(temp[i]);
    }
    list_free(temp);
}


#endif /*STRINGS_H*/
