#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

typedef struct matrix {
	char name; //�̸�
	double** elements; //��� �迭
	int row; //��
	int column; //��
}matrix;


FILE* file_pointer; //�Է� ���� ������
matrix* Matrix; //���Ͽ��� �ҷ��� ��� ����ü
matrix* ResultMatrix; //����� �����ϴ� ��� ����ü
int matrix_num; //������ ��� ����
char str[60]; //���� ���ڿ�
int cnt; //'+', '-' ������ ��Ÿ���� ����
int flag; //���� ���� �Ұ��� ���� �Ǵ� ����

void printMatrix(matrix M); //��� ��� �Լ�

void fetchMatrix(); //���� ��ġ �Լ�
void searchFile(); //�Է��� ������ �ִ��� ���θ� Ȯ��, �����ϴ� �Լ�
void checkMatrixNum(); //���� ���� ����� ������ ���� �Լ�
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
		printf("REF �Ǵ� RREF �� �ϳ��� ����ּ��� (����Ű: EXIT): ");
		scanf("%s", str);

		if (strcmp(str, "REF") == 0 || strcmp(str, "ref") == 0) {
			for (i = 0; i < matrix_num; i++) {
				topToBottom(ResultMatrix[i]);

				printf("��� %c: \n", Matrix[i].name);
				printMatrix(Matrix[i]);
				printf("��� %c�� REF ���: \n", ResultMatrix[i].name);
				printMatrix(ResultMatrix[i]);
				printf("--------------\n");
			}
		}
		else if (strcmp(str, "RREF") == 0 || strcmp(str, "rref") == 0) {
			for (i = 0; i < matrix_num; i++) {
				topToBottom(ResultMatrix[i]);
				bottomToTop(ResultMatrix[i]);

				printf("��� %c: \n", Matrix[i].name);
				printMatrix(Matrix[i]);
				printf("��� %c�� RREF ���: \n", ResultMatrix[i].name);
				printMatrix(ResultMatrix[i]);
				printf("--------------\n");
			}
		}
		else if (strcmp(str, "EXIT") == 0 || strcmp(str, "exit") == 0) {
			printf("����Ǿ����ϴ�.");
			break;
		}
		else {
			printf("�߸� �Է��ϼ̽��ϴ�.\n");
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
	
	while (i < M.row) { //Ƚ��
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
					num = multiple / M.elements[k][j]; //�� row�� �󸶸� ���ؾ� �ϴ���
					for (l = j; l < M.column; l++) {	
						M.elements[k][l] = M.elements[k][l] * num; //���� ��
					}
				}
			}

			for (k = i + 1; k < M.row; k++) { //���
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
			if (M.elements[i][j] - (int)M.elements[i][j] == 0) { //�����϶�
				if (M.elements[i][j] == -0 || M.elements[i][j] == 0) {
					printf("0\t");
				}
				else {
					printf("%.0f\t", M.elements[i][j]);
				}
			}
			else { //�Ǽ��϶�
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

		printf("��� %c = \n", Matrix[i].name);
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
		printf("Ȯ���ڸ�(.txt)�� ������ ���� �̸��� �Է��ϼ���: ");
		scanf("%s", file_name);

		strcat(file_name, ".txt");
		strcat(file_path, file_root);
		strcat(file_path, file_name);

		result = access(file_path, 0);

		if (result == 0) {
			printf("�ش� ������ �����ϹǷ� ����� �ҷ��ɴϴ�.\n");
			break;
		}
		else {
			printf("�ش� ������ �������� �ʽ��ϴ�.\n");
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
