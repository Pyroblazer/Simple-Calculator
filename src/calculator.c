#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

/*VARIABEL GLOBAL*/
char * x;
/*DEKLARASI FUNGSI DAN PROSEDUR*/
char *inputString(FILE* fp, size_t size);
int IsValidSyntax();
int KarToInteger(char c);
double complex count();
double complex salinDouble();
double complex tambah(double complex basis);
double complex kali(double complex basis);
double complex kurung();
double complex kurang(double complex basis);
double complex exponen(double complex basis);
double complex bagi(double complex basis);

int main()
/*Program utama secara garis besar dia akan menerima sebuah input dan melakukan validasi apakah syntax benar.
jika benar maka mengeluarkan hasil perhitungan, bisa terjadi Mengeluarkan "MATH ERROR" jika hasilnya infinity atau NAN.
jika salah maka mengeluarkan "SYNTAX ERROR" ke layar*/
// Input example: 
/* 123(1)+(1+2)+3*6+1/(((1+3)/4)+1) = 144.5
 * 5/0 = MATH ERROR */
{
	do{
	x = inputString(stdin, 10);
	if (*x == '\0')
	{
		printf("0\n");
	}
	else if (*x!='o' && *x!='e' && *x!='k')
	{
		if (IsValidSyntax()){
			double complex out=count();
			if (isfinite(creal(out)) && isfinite(cimag(out))){
				if (cimag(out) <(-0) ){
					if (creal(out)==0)
						printf("-%gi\n",-cimag(out));
					else
						printf("%g - %gi \n",creal(out),-cimag(out));
					
				}
				else if (cimag(out) ==0 || cimag(out)==-0)
					printf("%g\n",creal(out));
				else{
					if (creal(out)==0)
						printf("%gi\n",cimag(out));
					else
						printf("%g + %gi \n",creal(out),cimag(out));
				}
			}
			else 
				printf("MATH ERROR\n");
		}
		else{
			printf("SYNTAX ERROR\n");
		}
	}
	}while (*x!='o' && *x!='e' && *x!='k');
    return 0;
}

/*REALISASI PROSEDUR*/
char *inputString(FILE* fp, size_t size){
//pembacaan input string yang diletakkan pada sebuah array character dengan alokasi dinamik, sehingga tidak ada batas panjang string.
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

int IsValidSyntax(){
/*Mengecek apakah inputan sudah valid, dengan metode PDA namun stacknya diganti hanya dengan sebuah variabel, karena isi stack pasti tidak lebih dari 1 buah karakter.*/
	char* P = x;
	int jmlkurung=0, valid=1;
	char temp;
	if (*P == '\0') {
		return 1 ;
	}
	else if (*(P+1)=='\0'){
		valid = *P=='-'||(*P>='0'&&*P<='9')||*P=='i';
		return valid;
	}
	else{
		temp = *P;
		P++;
		if (temp!='.'){
		while (valid && *P!='\0') {
			if (temp=='(' && (*P=='/'||*P=='+' || *P=='*' || *P=='^' || *P==')' ))
				valid = 0;
			else if (temp=='(' && (*P=='-' || *P=='('||(*P>='0' && *P<='9') || *P=='i'))
				jmlkurung++;
			else if ((temp=='/'||temp =='+' || temp=='*' || temp=='^'|| temp=='-')&&(*P=='/'||*P=='+' || *P=='*' || *P=='^' || *P==')' || *P=='-'|| *P=='.'))
				valid =0;
			else if (temp==')')
				jmlkurung--;
			else if (temp=='.' && !(*P>='0' && *P<='9'))
				valid =0;
			else if (temp=='.'){
				while (*P>='0' && *P<='9')
					P++;
				valid = !(*P=='.');
			}
			else if (temp=='i' && ((*P>='0'&& *P<='9')||(*P=='.')||(*P=='i')))
				valid = 0;
			temp =*P;
			P++;
		}
		if (temp==')')
			jmlkurung--;
		else if (temp =='(')
			jmlkurung++;
		else if (temp == '*'|| temp=='/' || temp=='+' || temp =='-' || temp =='^')
			valid =0;
		return valid && jmlkurung==0;
		}
		else
			return 0;
	}
}
int KarToInteger(char c){
/*PREKONDISI inputan harus antara 0-9. 
fungsi untuk mengubah karakter menjadi integer*/
	switch (c) {
		case '0' :
			return 0;
		case '1' :
			return 1;
		case '2' :
			return 2;
		case '3' :
			return 3;
		case '4' :
			return 4;
		case '5' :
			return 5;
		case '6' :
			return 6;
		case '7' :
			return 7;
		case '8' :
			return 8;
		case '9' :
			return 9;
		default :
		{
			printf("SYNTAX ERROR\n");
			exit(0);
		}
	}
}
double complex salinDouble(){
/*PREKONDISI character yang sedang ditunjuk variabel global x adalah angka, huruf 'i', atau tanda negatif.
fungsi yang berguna untuk membaca sebuah inputan bilangan real atau imajiner*/
	char temp;
	double CInt=0;
	double complex retval;
	int neg=0;
	if (*x=='-'){
		neg=1;
		x++;
		while (*x>='0'&&*x<='9'){
		temp=*x;
		x++;
		if (*x>='0'&&*x<='9')
			CInt= (CInt + KarToInteger(temp))*10;
		else
			CInt= (CInt + KarToInteger(temp));
		}
		if (*x=='^')
			CInt = exponen(CInt);
	}
	if ((*x>='0'&&*x<='9')||*x=='i'){
	while (*x>='0'&&*x<='9'){
		temp=*x;
		x++;
		if (*x>='0'&&*x<='9')
			CInt= (CInt + KarToInteger(temp))*10;
		else
			CInt= (CInt + KarToInteger(temp));
	}
	if (*x=='.'){
		x++;
		int i=1;
		while (*x>='0'&&*x<='9'){
		CInt= (CInt + KarToInteger(*x)*pow(10,-i));
		i++;
		x++;
		if (*x=='i'){
			if (!neg)
				retval = 0+CInt*I;
			else
				retval = 0-CInt*I;
			x++;
			return retval;	
		}
	}
	}
	else if (*x=='i'){
		if (CInt==0){
			if (*(x-1)=='0')
				retval = 0+0*I;
			else{
				if (!neg){
					retval = 0+1*I;
				}
				else 
					retval = 0-1*I;
			}
		}
		else {
			if (!neg)
				retval = 0+CInt*I;
			else
				retval = 0-CInt*I;
		}
		x++;
		return retval;
	}
}else if (*x=='('){
	retval = kurung();
	if (!neg) {
		return retval;
	}
	else {
		return -retval;
	}
}
	if (!neg) {
		retval = CInt + 0*I;
		return retval;
	}
	else {
		retval = -CInt + 0*I;
		return retval;
	}
}
double complex kurung(){
/*PREKONDISI character pada indeks ke x adalah '('
menghasilkan nilai dari awal kurung sampai kurung berakhir.*/
	x++;
	double complex tmp=0,temp=0;
	if (*x=='('){
		tmp = kurung();
	}
	if ((*x>='0' && *x<='9')||*x=='-'||*x=='i')
		temp = salinDouble();
	else
		temp = tmp;
	while (*x!=')'){
		if (*x=='*')
			temp = kali(temp);
		else if (*x=='+')
			temp = tambah(temp);
		else if (*x=='-')
			temp = kurang(temp);
		else if (*x=='/')
			temp = bagi(temp);
		else if (*x=='^')
			temp = exponen(temp);
		else if (*x=='(')
			temp = temp*kurung();
	}
	x++;
	if (!((*x>='0' && *x<='9'))&&*x!='i'){
		return temp;
	}
	else {
		if (*x=='\0')
			return temp;
		else{
			tmp=salinDouble();
			return (temp*tmp);
		}
	}
	
		
}
double complex exponen(double complex basis){
/*PREKONDISI basis terdefinisi sebagai angka sebelum operator '^' dan isi indeks x sedang berada pada character '^'
fungsi ini akan mengembalikan nilai exponen dari basis dipangkatkan sesuatu angka dikanannya, bila angka dikanan ternyata bertemu eksponen maka proses akan direkursifkan, sampai betemu operator yang bukan eksponen*/
	x++;
	double complex tmp;
	if (*x=='('){
		tmp=kurung();
	}
	else
		tmp = salinDouble();
	if (*x!='^') {
		double complex hsl = cpow(basis,tmp);
		if (cimag(hsl)<0.00000000001)
			return creal(hsl)+0*I;
		else
			return hsl;
	}
	else{
		return cpow(basis,exponen(tmp));
	}
}
double complex tambah(double complex basis){
/*PREKONDISI basis terdefinisi sebagai angka sebelum operator '+' dan isi indeks x sedang berada pada character '+'
fungsi ini akan mengembalikan penambahan antara basis dengan angka dibelakang operator +. bisa jadi ada perkalian dan/atau pembagian dan/atau eksponen sehingga harus dikerjakan terlebih dahulu
*/
	x++;
	double complex tmp;
	if (*x=='('){
		tmp = kurung();
	}
	else
		tmp = salinDouble();
	if (*x!='+') {
		while (*x!='\0'&& *x!='-'&& *x!='+' && *x!=')'){
			if (*x=='^')
				tmp =exponen(tmp);
			else if (*x=='*')
				tmp = kali(tmp);
			else if (*x=='/')
				tmp = bagi(tmp);
		}
		return basis+tmp;
	}
	else{
		return basis+tambah(tmp);
	}
}

double complex kali(double complex basis){
/*PREKONDISI basis terdefinisi dan merupakan angka sebelum operator - dan isi indekx x adalah karakter '*'
fungsi ini mengembalikan hasil perkalian antara basis dengan angka disebelah kanan operan. bisa saja bertemu dengan eksponen, maka harus dikerjakan terlebih dahulu sampai eksponen selesai*/
	x++;
	double complex tmp;
	if (*x=='('){
		tmp=kurung();
	}
	else
		tmp = salinDouble();
	if (*x!='*') {
		if (*x=='^')
			return basis*exponen(tmp);
		else
			return basis*tmp;
	}
	else{
		return basis*kali(tmp);
	}
	
}
double complex bagi(double complex basis){
/*PREKONDISI basis terdefinisi dan merupakan angka sebelum operator - dan isi indekx x adalah karakter '/'
fungsi ini mengembalikan hasil pembagian antara basis dengan angka disebelah kanan operan. bisa saja bertemu dengan eksponen, maka harus dikerjakan terlebih dahulu sampai eksponen selesai*/
	x++;
	double complex tmp;
	if (*x=='('){
		tmp = kurung();
	}
	else
		tmp = salinDouble();

	if (*x!='/') {
		if (*x=='^')
			return basis/exponen(tmp);
		else
			return basis/tmp;
	}
	else{
		return basis/bagi(tmp);
	}
}
double complex kurang(double complex basis){
/*PREKONDISI basis terdefinisi dan merupakan angka sebelum operator - dan isi indekx x adalah karakter '-'
fungsi ini akan mengembalikan pengurangan antara basis dengan angka dibelakang operator -. bisa jadi ada perkalian dan/atau pembagian dan/atau eksponen sehingga harus dikerjakan terlebih dahulu*/
	x++;
	double complex tmp;
	if (*x=='('){
		tmp=kurung();
	}
	else
		tmp = salinDouble();
	if (*x!='-') {
		while (*x!='\0'&& *x!='-'&& *x!='+' && *x!=')'){
			if (*x=='^')
				tmp =exponen(tmp);
			else if (*x=='*')
				tmp = kali(tmp);
			else if (*x=='/')
				tmp = bagi(tmp);
		}
		return basis-tmp;
	}
	else{
		return basis-kurang(tmp);
	}
}
double complex count(){
/*PREKONDISI isi x tidak mungkin string yang tidak valid.
melakukan perhitungan hingga array habis, atau bertemu akhir dari string. dan mengembalikan hasil perhitungan tersebut.*/
	double complex tmp;
	if (*x!='(')
		tmp=salinDouble();
	else
		tmp=kurung();
	while (*x!='\0'){
		if (*x=='*')
			tmp = kali(tmp);
		else if (*x=='+')
			tmp = tambah(tmp);
		else if (*x=='-')
			tmp = kurang(tmp);
		else if (*x=='/')
			tmp = bagi(tmp);
		else if (*x=='^')
			tmp = exponen(tmp);
		else if (*x=='(')
			tmp = kurung() * tmp;
	}
	return tmp;
}
