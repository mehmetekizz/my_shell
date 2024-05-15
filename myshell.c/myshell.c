#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
 
#define INBUF_SIZE 256
 
int main()
{
    char inbuf[INBUF_SIZE] = {'\0'};
    int nbyte;
    
    printf("myshell id: %d\n", getpid());
    while(1) {
        write(1, "$", 1); 
        
        // Kullanici girdisini okur
        if ((nbyte = read(0, inbuf, INBUF_SIZE)) <= 1) {
            perror("input <= 0 byte");
        } else {
            inbuf[nbyte-1] = '\0'; // enterdan kaynakli yeni bir karakter algiliyor o yuzden o karakteri kaldiriyoruz
        }
        
        
        // "exit" komutu girilmisse programi sonlandır
        if (strncmp(inbuf, "exit", 4) == 0) {
            exit(0);
        }
        
        // Yeni bir islem baslatmak için fork kullan
        pid_t pid = fork();
        
        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            execlp(inbuf, inbuf, NULL); // Kullanıcının girdigi komutu çalıştır
            
            // Eğer execlp başarısız olursa hata mesajı yazdır
            perror("execlp");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            wait(NULL); // Child processin bitmesini bekle
        }
    }
    
    return 0;
}
