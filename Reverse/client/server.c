#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <keystone/keystone.h>
#include <keystone/x86.h>

int listenfd = 0, connectfd = 0;
struct sockaddr_in addr;
ks_engine* ks;
char* glob_ptr;
char* elements_ptr;

struct client_element_t
{
    char Data[0x100];
};

struct client_element_t global_data;

enum 
{
    OPT_NEW,
    OPT_DEL,
    OPT_EXEC,
    OPT_EXIT,
};

void _operation_new(int index, const char* str)
{
    unsigned char* result = NULL;
    size_t encoding_size, stat_count;
    if (ks_asm(ks, str, 0, &result, &encoding_size, &stat_count) != 0)
    {
        fprintf(stderr, "Failed to assemble %s\n", str);
        exit(1);
    }
    unsigned char opt = OPT_NEW; write(connectfd, &opt, sizeof(opt));
    write(connectfd, &index, sizeof(index));
    int size = encoding_size; write(connectfd, &size, sizeof(size));
    write(connectfd, result, encoding_size);
    ks_free(result);
}

void _operation_del(int index)
{
    unsigned char opt = OPT_DEL;
    write(connectfd, &opt, sizeof(opt));
    write(connectfd, &index, sizeof(index));
}

void _operation_exec(int index)
{
    unsigned char opt = OPT_EXEC;
    write(connectfd, &opt, sizeof(opt));
    write(connectfd, &index, sizeof(index));
}

void _operation_exit()
{
    unsigned char data = OPT_EXIT;
    write(connectfd, &data, sizeof(data));
}

void operation_dispatcher(unsigned char opt, int idx, const char* str)
{
    write(connectfd, &global_data, sizeof(global_data));
    switch (opt)
    {
    case OPT_NEW:
        _operation_new(idx, str);
        break;
    case OPT_DEL:
        _operation_del(idx);
        break;
    case OPT_EXEC:
        _operation_exec(idx);
        break;
    case OPT_EXIT:
    default:
        _operation_exit();
        break;
    }
    read(connectfd, &global_data, sizeof(global_data));
}

void operation_write_data(int index, const void* data, int len)
{
    write(connectfd, &global_data, sizeof(global_data));
    unsigned char opt = OPT_NEW;
    write(connectfd, &opt, sizeof(opt));
    write(connectfd, &index, sizeof(index));
    write(connectfd, &len, sizeof(len));
    write(connectfd, data, len);
    read(connectfd, &global_data, sizeof(global_data));
}

void operation_write_string(int index, const char* str)
{
    operation_write_data(index, str, strlen(str) + 1);
}

void operation_write_int64(int index, int64_t value)
{
    operation_write_data(index, &value, sizeof(value));
}

enum
{
    // RDI : item0
    // RSI : item1
    // RCX : item2
    // RAX : item3
    FUNC_MEMCPY = 400,  // Copy RCX bytes from RSI into RDI
    FUNC_MEMCMP,        // Compare RCX bytes between RSI and RDI, result in RAX
    FUNC_STRLEN,        // Calculate null terminated string's length in RDI, result in RAX 
};

void setup_builtin_functions()
{
    char asm_buffer[0x400] = {0};
    sprintf(asm_buffer, 
        "cld\n"
        "rep movsb\n"
        "ret"
    );
    operation_dispatcher(OPT_NEW, FUNC_MEMCPY, asm_buffer);
    puts("MEMCPY assembled");

    sprintf(asm_buffer, 
        "cld\n"
        "cmp rcx, rcx\n"
        "rep cmpsb\n"
        "setz al\n"
        "ret"
    );
    operation_dispatcher(OPT_NEW, FUNC_MEMCMP, asm_buffer);
    puts("MEMCMP assembled");

    sprintf(asm_buffer, 
        "xor rax, rax\n"
        "start:\n"
        "mov cl, byte ptr [rdi + rax]\n"
        "test cl, cl\n"
        "jz end\n"
        "inc rax\n"
        "jmp start\n"
        "end:\n"
        "ret"
    );
    operation_dispatcher(OPT_NEW, FUNC_STRLEN, asm_buffer);
    puts("STRLEN assembled");
}

void load_executable_functions()
{
    char asm_buffer[0x400] = {0};

    unsigned char answer[32] = 
    {
        0xd4, 0xe5, 0xbd, 0x70, 0x09, 0x20, 0xd2, 0xa3, 
        0x33, 0xbb, 0xb2, 0x7d, 0x47, 0x1b, 0xee, 0x74, 
        0x1c, 0xe4, 0x8b, 0x66, 0x15, 0xf5, 0x6e, 0xf2, 
        0x24, 0xbf, 0x39, 0xad, 0x62, 0x88, 0xb9, 0x71,
    };

    operation_write_data(7, answer, 32);
    operation_write_string(8, "Lab_Sign_1N_Key@");
    operation_write_string(9, "Please input: ");

    sprintf(asm_buffer,
        // write(STDOUT_FILENO, "Please input: ", 14)
        "mov rdi, 1\n"
        "mov rsi, %p + 0x900\n"
        "mov rdx, 15\n"
        "mov rax, 1\n"
        "syscall\n"

        // read(STDIN_FILENO, global_buffer, 32)
        "mov rdi, 0\n"
        "mov rsi, %p\n"
        "mov rdx, 32\n"
        "mov rax, 0\n"
        "syscall\n"
        "ret",
        elements_ptr,
        glob_ptr
    );
    operation_dispatcher(OPT_NEW, 10, asm_buffer);
    puts("iofunc assembled");

    sprintf(asm_buffer,
        "mov r9, rsi\n"
        "mov ecx, [rdi+4]\n"
        "mov esi, [rdi]\n"
        "mov r8, rdi\n"
        "xor eax, eax\n"
        "nop dword ptr [rax+00000000h]\n"
        "loop:\n"
        "mov edx, eax\n"
        "mov r10d, ecx\n"
        "and edx, 3\n"
        "shr r10d, 5\n"
        "mov edi, [r9+rdx*4]\n"
        "mov edx, ecx\n"
        "shl edx, 4\n"
        "xor edx, r10d\n"
        "add edi, eax\n"
        "sub eax, 61C88647h\n"
        "add edx, ecx\n"
        "xor edx, edi\n"
        "add esi, edx\n"
        "mov edx, eax\n"
        "shr edx, 0Bh\n"
        "mov r10d, esi\n"
        "and edx, 3\n"
        "shr r10d, 5\n"
        "mov edi, [r9+rdx*4]\n"
        "mov edx, esi\n"
        "shl edx, 4\n"
        "xor edx, r10d\n"
        "add edi, eax\n"
        "add edx, esi\n"
        "xor edx, edi\n"
        "add ecx, edx\n"
        "cmp eax, 0C6EF3720h\n"
        "jnz loop\n"
        "mov [r8], esi\n"
        "mov [r8+4], ecx\n"
        "ret"
    );
    operation_dispatcher(OPT_NEW, 11, asm_buffer);
    puts("encrypt assembled");

    sprintf(asm_buffer,
        "mov r9, rsi\n"
        "mov ecx, [rdi+4]\n"
        "mov esi, [rdi]\n"
        "mov r8, rdi\n"
        "mov eax, 0C6EF3720h\n"
        "nop dword ptr [rax+00h]\n"
        "loop:\n"
        "mov edx, eax\n"
        "mov r10d, esi\n"
        "shr edx, 0Bh\n"
        "shr r10d, 5\n"
        "and edx, 3\n"
        "mov edi, [r9+rdx*4]\n"
        "mov edx, esi\n"
        "shl edx, 4\n"
        "xor edx, r10d\n"
        "add edi, eax\n"
        "add eax, 61C88647h\n"
        "add edx, esi\n"
        "xor edx, edi\n"
        "sub ecx, edx\n"
        "mov edx, eax\n"
        "and edx, 3\n"
        "mov r10d, ecx\n"
        "mov edi, [r9+rdx*4]\n"
        "mov edx, ecx\n"
        "shr r10d, 5\n"
        "shl edx, 4\n"
        "xor edx, r10d\n"
        "add edi, eax\n"
        "add edx, ecx\n"
        "xor edx, edi\n"
        "sub esi, edx\n"
        "test eax, eax\n"
        "jnz loop\n"
        "mov [r8], esi\n"
        "mov [r8+4], ecx\n"
        "ret"
    );
    operation_dispatcher(OPT_NEW, 12, asm_buffer);
    puts("decrypt assembled");

    sprintf(asm_buffer,
        "mov rax, %p + 11 * 0x100\n" // encrypt(flag, key)
        "mov rdi, %p\n"
        "mov rsi, %p + 0x800\n"
        "call rax\n"
        "mov rax, %p + 11 * 0x100\n" // encrypt(flag + 8, key)
        "mov rdi, %p + 8\n"
        "mov rsi, %p + 0x800\n"
        "call rax\n"
        "mov rax, %p + 12 * 0x100\n" // decrypt(flag + 16, key)
        "mov rdi, %p + 16\n"
        "mov rsi, %p + 0x800\n"
        "call rax\n"
        "mov rax, %p + 12 * 0x100\n" // decrypt(flag + 24, key)
        "mov rdi, %p + 24\n"
        "mov rsi, %p + 0x800\n"
        "call rax\n"
        "mov rdi, %p + 0x700\n" // memcmp(answer, flag, 32)
        "mov rsi, %p\n"
        "mov rcx, 32\n"
        "mov rax, %p + 401 * 0x100\n"
        "call rax\n"
        "ret",
        elements_ptr, glob_ptr, elements_ptr, elements_ptr, glob_ptr,
        elements_ptr, elements_ptr, glob_ptr, elements_ptr, elements_ptr,
        glob_ptr, elements_ptr, elements_ptr, glob_ptr, elements_ptr
    );
    operation_dispatcher(OPT_NEW, 13, asm_buffer);
    puts("validation assembled");

    sprintf(asm_buffer,
        "mov rax, %p + 13 * 0x100\n"
        "call rax\n"
        "test al, al\n"
        "jnz right\n"
        "wrong:\n"
        "mov rdi, 1\n"
        "mov rsi, %p + 300 * 0x100\n"
        "mov rdx, 48\n"
        "mov rax, 1\n"
        "syscall\n"
        "ret\n"
        "right:\n"
        "mov rdi, 1\n"
        "mov rsi, %p + 200 * 0x100\n"
        "mov rdx, 49\n"
        "mov rax, 1\n"
        "syscall\n"
        "ret",
        elements_ptr,
        elements_ptr,
        elements_ptr
    );
    operation_dispatcher(OPT_NEW, 14, asm_buffer);
    puts("main assembled");

    operation_write_string(200, "Congratulations, you have sign in successfully!\n");
    operation_write_string(300, "Failed to verify your input! Please try again!\n");
}

void run_validation()
{
    operation_dispatcher(OPT_EXEC, 10, NULL);
    for (int i = 0; i < 32; ++i)
        global_data.Data[i] ^= 0x1F;
    operation_dispatcher(OPT_EXEC, 14, NULL);
    operation_dispatcher(OPT_EXIT, 0, NULL);
}

int main()
{
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(11451);

    bind(listenfd, (struct sockaddr*)&addr, sizeof(addr));

    listen(listenfd, 1);

    ks_open(KS_ARCH_X86, KS_MODE_64, &ks);
    while (1)
    {
        connectfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        puts("Connected.");

        read(connectfd, &glob_ptr, sizeof(glob_ptr));
        read(connectfd, &elements_ptr, sizeof(elements_ptr));

        setup_builtin_functions();
        load_executable_functions();

        run_validation();

        close(connectfd);
        puts("Closed.");
    }
    ks_close(ks);

    return 0;
}