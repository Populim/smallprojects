#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){

	//essa entrada do angulo eu to deixando assim pra ficar mais fácil da gente testar
	//não tem problema deixar o angulo hardcoded no vhdl/verilog
	int degree;
	printf("Em quantos graus você gostaria de rotacionar a imagem no sentido horário?\n");
	scanf("%d",&degree);

	if(degree < 0){
		degree *= -1;
		degree += 180;
	}
	if(degree%90 != 0){
		printf("Digite um ângulo multiplo de 90\n");
		return 0;
	}
	if (degree>360){
		degree = degree%360;
	}
	if(degree == 0){
		return 0;
	}


	FILE *fp = fopen("newcat.pgm", "rb");
	char input;
	int count = 0;
	int width = 0;
	int height = 0;
	int shades = 0;
	int aux;
	unsigned char* v2;
	unsigned char* v1;
	while(1){
		fread(&input,1,1,fp);
		// espaço				\n 			tab			\r
		if(input == 32 ||input == 10 ||input == 9 ||input == 13){
			count++;
			if(count == 4){
				break;
			}
			continue;
		}
		if(count == 1){
			width = width*10;
			width += input-48;
		}
		if(count == 2){
			height = height*10;
			height += input-48;
		}
		if(count == 3){
			shades = shades*10;
			shades += input-48;
		}
	}
	printf("Lidos:\n");
	printf("Width:%d Height:%d Shades:%d\n\n", width,height,shades);


	v1 = malloc(width*height*sizeof(char));
	v2 = malloc(width*height*sizeof(char));
	for (int i = 0; i < width*height; i++){
		fread(&v1[i],1,1,fp);
	}
	fclose(fp);




	if(degree == 90){
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){
				v2[i*height+j] = v1[(height-j-1)*width+i];
				
			}
		}//v2[i*height+j] = v1[(height-j-1)*width+i]				
	}
	else if (degree == 180){
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){
				v2[i*height+j] = v1[(width-i-1)*height+(height-j-1)];
			}
		}
	}
	else if (degree == 270){
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){
				v2[i*height+j] = v1[(j)*width+(width-i-1)];			}
		}//v2[i*height+j] = v1[(height-j-1)*width+i]		
	}


	// for (int i = 0; i < width; i++){
	// 	for (int j = 0; j < height; j++){
	// 		//v2[i*height+j] = v1[(j)*width+(width-i-1)];
	// 	}
	// }//v2[i*height+j] = v1[(height-j-1)*width+i]

	// for (int i = 0; i < width; i++){
	// 	for (int j = 0; j < height; j++){
	// 		//v2[i*height+j] = v1[(width-i-1)*height+(height-j-1)];
	// 	}
	// }
	char nomearq[30];
	sprintf(nomearq,"pgmrotated%d.pgm",degree);
	fp = fopen(nomearq, "wb");
	unsigned char cabecalho[10] = "P5\n";
	unsigned char aux2;
	unsigned char space = ' ';
	unsigned char barran = '\n';
	int dimensao;
	fwrite(cabecalho,1,3,fp);


	if(degree%180 == 0){
		aux = height;
		height = width;
		width = aux;
	}
	for (int i = 5; i >= 0; i--){
		aux = width/(int)pow(10,i);
		if(aux == 0){
			continue;
		}
		else{
			aux = aux%10;
			aux2 = (char)aux;
			aux2 += 48;
			printf("%c", aux2);
			fwrite(&aux2,1,1,fp);
		}
	}
	printf("\n");
	fwrite(&space,1,1,fp);

	for (int i = 5; i >= 0; i--){
		aux = height/(int)pow(10,i);
		if(aux == 0){
			continue;
		}
		else{
			aux = aux%10;
			aux2 = (char)aux;
			aux2 += 48;
			printf("%c", aux2);
			fwrite(&aux2,1,1,fp);
		}
	}
	printf("\n");


	fwrite(&barran,1,1,fp);
	for (int i = 5; i >= 0; i--){
		aux = (int)(shades/(int)pow(10,i));
		if(aux == 0){
			continue;
		}
		else{
			aux = aux%10;
			aux2 = (char)aux;
			aux2 += 48;
			printf("%c", aux2);
			fwrite(&aux2,1,1,fp);
		}
	}
	printf("\n");
	fwrite(&barran,1,1,fp);

	for (int i = 0; i < width*height; i++){
		fwrite(&(v2[i]),1,1,fp);
	}

	printf("%d %d %d\n", width,height,shades);

	fclose(fp);
	free(v1);
	free(v2);
}