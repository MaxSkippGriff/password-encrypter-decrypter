/*
   Simple Password Encrytper/decrypter
   -----------------------------------

   This is a very basic password encryption/decryption
   program. It uses an XOR cipher to encrypt the password
   data. This works in the following way. First a random key
   is generated. Then, XOR is done using that key to create
   an encrypted output. To decrypt this output, the XOR
   operation is run again, that is, you only write a 1 if the
   bits are exclusively different, for e.g.,

   1 0 0 1
   0 1 0 1
   -------
   1 1 0 0

   This is very basic, and requires refinement.

   First, you'll be prompted to enter your name and
   password. Then you'll be shown the name, key and the
   encryption output of your password. This data is stored
   in the file "oldPass.txt".

   I have included a delete function to delete passwords
   after they've been used.

   Continue this process and try out different passwords.
   It's a little bit laborous, but that's the nature of the
   password manager beast.

   NB:- Please don't use special characters - the password
   decryption only decodes upper/lowercase letters and numbers.

*/


#include "password.h"



int main (int argc, char** argv)
{
   start_password();

   return 0;
}

/*
   Starts the password function.
*/
void start_password(void)
{
   save_password();

   delete_pass();

}

/*
	  A random string is generated using the key generator. The
   key is then passed to 'decryption' and the resulting string is
   written to the password store.
*/
void save_password(void)
{
   FILE *fp;
   char password[100], ref[100], encrypt[100], key[100], decrypt[100];

   printf("\nEnter your name: ");
   scanf("%s", ref);
   printf("Enter your password: ");
   scanf("%s", password);

   key_generator(key,strlen(password));
   decryption(password, encrypt, key);

   printf("\nName: %s", ref);
   printf("\nKey: %s", key);
   printf("\nEncrypted: %s", encrypt);

   fp=fopen("oldPass.txt","a");
   if(fp == NULL){
	     printf("Searching...can't find anything.\n");
   }
   else{
	     fprintf(fp,"%s %s %s", ref, encrypt, key);
   }
   fclose(fp);
   printf("\n\nYour password has been encrypt and saved.\n");

   open_password(password, ref, encrypt, key, decrypt);
}

/*
   After prompting the user to enter the ref, password vault
   is opened and the refs are compared. When found, the encrypt
   string and the key is passed to decryption() for decrypting
*/
void open_password(char password[100], char ref[100], char encrypt[100],
                   char key[100], char decrypt[100])
{
   int i;
   char name[40];
   FILE *fp;

   printf("\nEnter your name: ");
   scanf("%s", name);

   fp=fopen("oldPass.txt","r");
   if(fp == NULL){
      printf("File failed to open");
   }
   else{
      for(i=0;i<40;i++){
          fscanf(fp,"%s %s %s \n", ref, encrypt, key);
          if(strcmp(ref, name)==0){
             decryption(encrypt, decrypt, key);
          }
      }
   }
   printf("Your decrypt password is: %s", decrypt);

   display_names(password, ref, encrypt, key, decrypt);
}

/*
	  The password store is read and the references are printed
   unit the end of file.
*/

void display_names(char password[100], char ref[100], char encrypt[100],
                   char key[100], char decrypt[100])
{
   FILE *file;

   printf("\nThe name associated is: ");

   file=fopen("oldPass.txt","r");

   if(file == NULL){
      printf("File failed to open");
   }
   else{
      while(!feof(file)){
         fscanf(file, "%s %s %s \n", ref, encrypt, key);
         printf("%s", ref);
      }
   }

    start_program(password, ref, encrypt, key, decrypt);
}

/*
   With the help of rand() from stdlib.h, a random character
   from the character array is taken is repeated n times to
   create a random string of length n.
*/
extern void key_generator(char *key, int n)
{
   int index = 0;
   char strings[100] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

   srand((unsigned int)(time(NULL)));

   for(index = 0; index < n; index++){
      key[index] = strings[rand() % (sizeof strings - 1)];
   }

   key[index]='\0';
}

/*
	  Decryption implements the XOR cipher. The result is stored into
   output.
*/
void decryption(char *input, char *output, char *key)
{
   int i;
   int len;

   len = strlen(input);

   for(i = 0; i < len; i++){
      output[i] = input[i] ^ key[i];
   }
   output[i]='\0';
}



void start_program(char password[100], char ref[100], char encrypt[100],
                  char key[100], char decrypt[100])
{
   char name[40];
   FILE *fp;
   int i;

  	printf("\nTo start the program, enter your name: ");
  	scanf("%s", name);
   printf("Enter your password: ");
  	scanf("%s", password);

   fp=fopen("oldPass.txt","r");

   if(fp == NULL){
      printf("\nFile failed to open");
   }
   else{
      for(i=0;i<40;i++){
         fscanf(fp,"%s %s %s \n", ref, encrypt, key);
         if(strcmp(ref, name)==0){
            break;
         }
      }
   }
   decryption(encrypt, decrypt, key);

   while(strcmp(password, decrypt)!=0){
      printf("Incorrect entry. Please try again: ");
      scanf("%s", password);
   }
   printf("Decrypted password %s matches password %s. \n",
          decrypt, password);
}


/*
	  Deletes the password store.
*/
void delete_pass()
{
   if(remove("oldPass.txt") == 0){
      printf("Password deleted successfully.\n");
   }
   else{
    	printf("Unable to delete the vault");
   }
}