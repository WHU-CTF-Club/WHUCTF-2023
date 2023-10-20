#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

struct client_element_t
{
    char Data[0x100];
};

int fd = 0;
struct client_element_t* elements = NULL;
struct client_element_t global_data = {0};

void client_init();
void client_fini();
void client_operation_new();
void client_operation_del();
void client_operation_execute();
void client_ragequit(int code);
void client_sync_global_send();
void client_sync_global_recv();

int main()
{
    client_init();
    if (elements == NULL)
        client_ragequit(1);

    struct sockaddr_in addr;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        client_ragequit(1);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(11451);
    
    // const char* server_ip = "125.220.147.49";
    const char* server_ip = "127.0.0.1";
    if (inet_pton(AF_INET, server_ip, &addr.sin_addr) <= 0)
        client_ragequit(1);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        client_ragequit(1);

    const char* glob_ptr = global_data.Data;
    if (write(fd, &glob_ptr, sizeof(&glob_ptr)) != sizeof(&glob_ptr))
        client_ragequit(1);
    if (write(fd, &elements, sizeof(&elements)) != sizeof(&elements))
        client_ragequit(1);

    bool loop = true;
    while (loop)
    {
        client_sync_global_recv();
        unsigned char operation = 0;
        if (read(fd, &operation, sizeof(operation)) != sizeof(operation))
            client_ragequit(1);

        enum 
        {
            OPT_NEW,
            OPT_DEL,
            OPT_EXEC,
            OPT_EXIT,
        };
        switch (operation)
        {
        case OPT_NEW:
            client_operation_new();
            break;
        case OPT_DEL:
            client_operation_del();
            break;
        case OPT_EXEC:
            client_operation_execute();
            break;
        case OPT_EXIT:
        default:
            loop = false;
            break;
        }
        client_sync_global_send();
    }

    client_fini();

    return 0;
}

void client_init()
{
    elements = mmap(NULL, sizeof(struct client_element_t) * 500, PROT_READ | PROT_WRITE | PROT_EXEC, 0x20 | MAP_PRIVATE, -1, 0);
    mprotect(elements, sizeof(struct client_element_t) * 500, PROT_READ | PROT_WRITE | PROT_EXEC);
    memset(elements, 0xC3, sizeof(struct client_element_t) * 500);
}

void client_fini()
{
    munmap(elements, sizeof(struct client_element_t) * 500);
}

void client_operation_new()
{
    int store_index;
    
    if (read(fd, &store_index, sizeof(store_index)) != sizeof(store_index))
        client_ragequit(1);
    if (store_index < 0 || store_index >= 500)
        client_ragequit(1);
    
    int code_length;
    if (read(fd, &code_length, sizeof(code_length)) != sizeof(code_length))
        client_ragequit(1);
    
    if (code_length < 0 || code_length >= sizeof(struct client_element_t))
        client_ragequit(1);

    if (read(fd, elements[store_index].Data, code_length) != code_length)
        client_ragequit(1);
}

void client_operation_del()
{
    int store_index;
    if (read(fd, &store_index, sizeof(store_index)) != sizeof(store_index))
        client_ragequit(1);
    
    if (store_index < 0 || store_index >= 500)
        client_ragequit(1);
    
    memset(elements[store_index].Data, 0xC3, sizeof(struct client_element_t));
}

void client_operation_execute()
{
    int store_index;
    if (read(fd, &store_index, sizeof(store_index)) != sizeof(store_index))
        client_ragequit(1);
    
    if (store_index < 0 || store_index >= 500)
        client_ragequit(1);
    
    typedef void(func_t)(void);
    func_t* function = (func_t*)&elements[store_index];
    function();
}

void client_ragequit(int code)
{
    if (errno != 0)
        perror("Rage quit because");
    if (fd != 0)
        close(fd);
    client_fini();
    exit(code);
}

void client_sync_global_send()
{
    if (write(fd, &global_data, sizeof(global_data)) != sizeof(global_data))
        client_ragequit(1);
}

void client_sync_global_recv()
{
    if (read(fd, &global_data, sizeof(global_data)) != sizeof(global_data))
        client_ragequit(1);
}