#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

typedef struct matrix {
	char name; //이름
	double** elements; //행렬 배열
	int row; //행
	int column; //열
}matrix;


FILE* file_pointer; //입력 파일 포인터
matrix* Matrix; //파일에서 불러온 행렬 구조체
matrix* ResultMatrix; //결과를 저장하는 행렬 구조체
int matrix_num; //파일의 행렬 개수
char str[60]; //연산 문자열
int cnt; //'+', '-' 개수를 나타내는 정수
int flag; //연산 가능 불가능 여부 판단 정수

void printMatrix(matrix M); //행렬 출력 함수

void fetchMatrix(); //파일 패치 함수
void searchFile(); //입력한 파일이 있는지 여부를 확인, 동작하는 함수
void checkMatrixNum(); //파일 안의 행렬의 개수를 세는 함수
void setResultMatrix();
void topToBottom();
void bottomToTop();
void divide(matrix M);
void makeElementOne(matrix M);
int GDC(int a, int b);

void main() {
	int i;
	char str[10] = "";
	matrix_num = 0;

	searchFile();

	while (1) {
		setResultMatrix();
		printf("REF 또는 RREF 중 하나를 골라주세요 (종료키: EXIT): ");
		scanf("%s", str);

		if (strcmp(str, "REF") == 0 || strcmp(str, "ref") == 0) {
			for (i = 0; i < matrix_num; i++) {
				topToBottom(ResultMatrix[i]);

				printf("행렬 %c: \n", Matrix[i].name);
				printMatrix(Matrix[i]);
				printf("행렬 %c의 REF 결과: \n", ResultMatrix[i].name);
				printMatrix(ResultMatrix[i]);
				printf("--------------\n");
			}
		}
		else if (strcmp(str, "RREF") == 0 || strcmp(str, "rref") == 0) {
			for (i = 0; i < matrix_num; i++) {
				topToBottom(ResultMatrix[i]);
				bottomToTop(ResultMatrix[i]);

				printf("행렬 %c: \n", Matrix[i].name);
				printMatrix(Matrix[i]);
				printf("행렬 %c의 RREF 결과: \n", ResultMatrix[i].name);
				printMatrix(ResultMatrix[i]);
				printf("--------------\n");
			}
		}
		else if (strcmp(str, "EXIT") == 0 || strcmp(str, "exit") == 0) {
			printf("종료되었습니다.");
			break;
		}
		else {
			printf("잘못 입력하셨습니다.\n");
		}
	}
	

	return;
}

void setResultMatrix() {
	int i, j, k;

	for (i = 0; i < matrix_num; i++) {
		for (j = 0; j < Matrix[i].row; j++) {
			for (k = 0; k < Matrix[i].column; k++) {
				ResultMatrix[i].elements[j][k] = Matrix[i].elements[j][k];
			}
		}
	}

	return;
}

void divide(matrix M) {
	int i, j;
	int result;

	for (i = 0; i < M.row; i++) {
		result = M.elements[i][0];
		for (j = 1; j < M.column; j++) {
			result = GDC(result, M.elements[i][j]);
		}
		if (result == 0) {
			result = 1;
		}
		for (j = 0; j < M.column; j++) {
			M.elements[i][j] = M.elements[i][j] / result;
		}
	}

	return;
}
int GDC(int a, int b) {
	int i, j;
	int tmp = 1;

	if (a == b) {
		tmp = a;
	}
	if (a == 0 && b != 0) {
		tmp = b;
	}
	else if (a != 0 && b == 0) {
		tmp = a;
	}
	else if (a == 0 && b == 0) {
		tmp = 0;
	}
	else {
		if (a < b) {
			j = a;
		}
		else {
			j = b;
		}

		for (i = 1; i <= j; i++) {
			if (a % i == 0 && b % i == 0) {
				tmp = i;
			}
		}
	}

	return tmp;
}

void makeElementOne(matrix M) {
	int i = 0, j = 0, k = 0;
	int num = 1;

	while (i < M.row) {
		j = 0;
		num = 1;
		while (j < M.column) {
			if (M.elements[i][j] != 0) {
				num = M.elements[i][j];
				M.elements[i][j] = M.elements[i][j] / num;
				break;
			}
			j++;
		}
		for (k = j + 1; k < M.column; k++) {
			if (M.elements[i][k] != 0) {
				(double)M.elements[i][k] = (double)M.elements[i][k] / (double)num;
			}
		}
		i++;
	}

	return;
}

void topToBottom(matrix M) {
	int multiple = 1;
	int num = 1;
	double tmp;
	int i = 0, j = 0, k = 0, l = 0, m = 0;
	
	while (i < M.row) { //횟수
		j = 0;
		while (j < M.column) { //
			multiple = 1;

			while (M.elements[i][j] == 0) {
				j++;
			}
			
			for (k = i; k < M.row; k++) {
				m = M.row - 1;
				if (M.elements[k][j] == 0) {
					if (m <= k) {
						break;
					}
					if (M.elements[m][j] == 0) {
						m--;
					}
					for (l = 0; l < M.column; l++) {
						tmp = M.elements[k][l];
						M.elements[k][l] = M.elements[m][l];
						M.elements[m][l] = tmp;
					}
				}
			}

			for (k = i; k < M.row; k++) {
				if (M.elements[k][j] != 0) {
					multiple = multiple * M.elements[k][j];
				}
			}

			for (k = i; k < M.row; k++) {
				if (M.elements[k][j] != 0) {
					num = multiple / M.elements[k][j]; //각 row에 얼마를 곱해야 하는지
					for (l = j; l < M.column; l++) {	
						M.elements[k][l] = M.elements[k][l] * num; //곱한 값
					}
				}
			}

			for (k = i + 1; k < M.row; k++) { //계산
				if (M.elements[k][j] != 0) {
					for (l = j; l < M.column; l++) {
						M.elements[k][l] = M.elements[k][l] - M.elements[i][l];
					}
				}
			}
			j++;
			break;
		}
		i++;
	}
	divide(M);
	return;
}

void bottomToTop(matrix M) {
	int multiple = 1;
	int num = 1;
	int i = 0, j = 0, k = 0, l = 0;

	while (i < M.row) {
		while (j < M.column) {
			multiple = 1;

			while (M.elements[i][j] == 0) {
				j++;
			}

			for (k = i; k >= 0; k--) {
				if (M.elements[k][j] != 0) {
					multiple = multiple * M.elements[k][j];
				}
			}

			for (k = i; k >= 0; k--) {
				if (M.elements[k][j] != 0) {
					num = multiple / M.elements[k][j];
					for (l = j; l < M.column; l++) {
						M.elements[k][l] = M.elements[k][l] * num;
					}
				}
			}

			for (k = i - 1; k >= 0; k--) {
				if (M.elements[k][j] != 0) {
					for (l = 0; l < M.column; l++) {
						M.elements[k][l] = M.elements[k][l] - M.elements[i][l];
					}
				}
			}
			j++;
			break;
		}
		i++;
	}

	divide(M);
	makeElementOne(M);
	return;
}

void printMatrix(matrix M) {
	int i, j;
	
	for (i = 0; i < M.row; i++) {
		for (j = 0; j < M.column; j++) {
			if (M.elements[i][j] - (int)M.elements[i][j] == 0) { //정수일때
				if (M.elements[i][j] == -0 || M.elements[i][j] == 0) {
					printf("0\t");
				}
				else {
					printf("%.0f\t", M.elements[i][j]);
				}
			}
			else { //실수일때
				printf("%.1f\t", M.elements[i][j]);
			}
		}
		printf("\n");
	}

	return;
}

void checkMatrixNum() {
	char find_alphabet;

	while (!feof(file_pointer)) {
		fscanf(file_pointer, "%c", &find_alphabet);

		if (find_alphabet >= 'A' && find_alphabet <= 'Z') {
			matrix_num++;
		}
	}
	return;
}
void fetchMatrix() {
	int i, j, k;
	int tmp;
	char nothing;

	for (i = 0; i < matrix_num; i++) {
		fscanf(file_pointer, "%c", &Matrix[i].name);
		ResultMatrix[i].name = Matrix[i].name;

		fscanf(file_pointer, "%c", &nothing);

		fscanf(file_pointer, "%d", &Matrix[i].row);
		ResultMatrix[i].row = Matrix[i].row;

		fscanf(file_pointer, "%c", &nothing);

		fscanf(file_pointer, "%d", &Matrix[i].column);
		ResultMatrix[i].column = Matrix[i].column;

		Matrix[i].elements = (double**)malloc(sizeof(double*) * Matrix[i].row);
		ResultMatrix[i].elements = (double**)malloc(sizeof(double*) * ResultMatrix[i].row);
		for (j = 0; j < Matrix[i].row; j++) {
			Matrix[i].elements[j] = (double*)malloc(sizeof(double) * Matrix[i].column);
			ResultMatrix[i].elements[j] = (double*)malloc(sizeof(double) * ResultMatrix[i].column);
		}

		for (j = 0; j < Matrix[i].row; j++) {
			for (k = 0; k < Matrix[i].column; k++) {
				fscanf(file_pointer, "%d", &tmp);

				Matrix[i].elements[j][k] = tmp;
				ResultMatrix[i].elements[j][k] = Matrix[i].elements[j][k];

				fscanf(file_pointer, "%c", &nothing);
			}
		}

		printf("행렬 %c = \n", Matrix[i].name);
		printMatrix(Matrix[i]);
		printf("--------------\n");
	}

	return;
}
void searchFile() {
	char file_root[10] = "\0";
	char file_path[100] = "\0";
	char file_name[20] = "\0";
	int result;

	strcat(file_root, "./");

	while (1) {
		printf("확장자명(.txt)을 제외한 파일 이름을 입력하세요: ");
		scanf("%s", file_name);

		strcat(file_name, ".txt");
		strcat(file_path, file_root);
		strcat(file_path, file_name);

		result = access(file_path, 0);

		if (result == 0) {
			printf("해당 파일이 존재하므로 행렬을 불러옵니다.\n");
			break;
		}
		else {
			printf("해당 파일이 존재하지 않습니다.\n");
			file_path[0] = '\0';
		}
	}

	file_pointer = fopen(file_path, "r");
	checkMatrixNum();
	fclose(file_pointer);

	Matrix = (matrix*)malloc(sizeof(matrix) * matrix_num);
	ResultMatrix = (matrix*)malloc(sizeof(matrix) * matrix_num);

	file_pointer = fopen(file_path, "r");
	fetchMatrix();
	fclose(file_pointer);

	return;
}
