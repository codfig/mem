#include <cairo.h>
#include <cairo-pdf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LARG_A4 595
#define ALT_A4 842

int main(int argc, char** argv) {

    int n_cols, n_cels, end_inicial;
    bool centralizado, numerado;
    char entrada[32];

    printf("Informe o número de colunas, variando de 1 a 4 (Digitar ENTER vale 3): ");
    fgets(entrada, sizeof entrada, stdin);
    if (entrada[0] == '\n') {
        n_cols = 3;
    } else {
        n_cols = atoi(entrada);
        while (n_cols<1 || n_cols>4) {
            printf("VALOR INVÁLIDO! O número deve ser 1, 2 ou 3. (Digitar ENTER vale 2 | Digitar 0 encerra): ");
            fgets(entrada, sizeof entrada, stdin);
            if (entrada[0] == '\n') {
                n_cols = 3;
            } else {
                n_cols = atoi(entrada);
                if (n_cols == 0) {
                    exit(0);
                }
                 entrada[0] = '0';
           }
        }
    }

    #define INF_CELLS 40
    #define DFT_CELLS 45
    #define SUP_CELLS 50
    printf("Informe o número de células por coluna, variando de %d a %d (Digitar ENTER vale %d): ", INF_CELLS, SUP_CELLS, DFT_CELLS);
    fgets(entrada, sizeof entrada, stdin);
    if (entrada[0] == '\n') {
        n_cels = DFT_CELLS;
    } else {
        n_cels = atoi(entrada);
        while (n_cels<INF_CELLS || n_cels>SUP_CELLS) {
            printf("VALOR INVÁLIDO! O número deve estar entre %d e %d (Digitar ENTER vale %d | Digitar 0 encerra): ", INF_CELLS, SUP_CELLS, DFT_CELLS);
            fgets(entrada, sizeof entrada, stdin);
            if (entrada[0] == '\n') {
                n_cels = 45;
            } else {
                n_cels = atoi(entrada);
                if (n_cels == 0) {
                    exit(0);
                }
                entrada[0] = '0';
            }
        }
    }

    srand(time(NULL));
    int sorteio = (rand() % 451) * 10 + 4900;   // sorteia algum entre: 4900, 4910, 4920, 4930, ... 9300

    printf("Informe o endereço inicial. 1 vale 4800, 2 vale %d ou digite valor entre 4800 e 9300 (Digitar ENTER vale %d: ", sorteio, sorteio);
    fgets(entrada, sizeof entrada, stdin);
    if (entrada[0] == '1') {
        end_inicial = 4800;
    } else if (entrada[0] == '2' || entrada[0] == '\n') {
        end_inicial = sorteio;
    } else {
        end_inicial = atoi(entrada);
        while (end_inicial<4800 || end_inicial>9300) {
            printf("VALOR INVÁLIDO! 0=Encerra | 1=4800 | 2=%d | ENTER=%d | ou qualquer valor entre 4800 e 9300: ", sorteio, sorteio);
            fgets(entrada, sizeof entrada, stdin);
            if (entrada[0] == '1') {
                end_inicial = 4800;
            } else if (entrada[0] == '2' || entrada[0] == '\n') {
                end_inicial = sorteio;
            } else {
                end_inicial = atoi(entrada);
                if (end_inicial == 0) {
                    exit(0);
                }
                entrada[0] = '0';
            }
        }
    }

    printf("Ampliar a margem direita? (S/N, ENTER=N): ");
    fgets(entrada, 2, stdin);
    char ch = entrada[0];
    if (ch == 'S' || ch == 's') {
        centralizado = false;
    } else if (ch == 'N' || ch == 'n' || ch == '\n') {
        centralizado = true;
    } else {
        while (ch != 'S' && ch != 's' && ch != 'N' && ch != 'n') {
            printf("CARACTER INVÁLIDO! Digite S ou N ou 0 para encerrar: ");
            scanf(" %c", &ch);
                if (ch == 'S' || ch == 's') {
                    centralizado = false;
                } else if (ch == 'N' || ch == 'n' || ch == '\n') {
                    centralizado = true;
                } else if (ch == '0') {
                    exit(0);
                }
        }
    }

    printf("Mostrar os endereços apenas na primeira coluna? (S/N, ENTER=N): ");
    fgets(entrada, 2, stdin);
    ch = entrada[0];
    if (ch == 'S' || ch == 's') {
        numerado = false;
    } else if (ch == 'N' || ch == 'n' || ch == '\n') {
        numerado = true;
    } else {
        while (ch != 'S' && ch != 's' && ch != 'N' && ch != 'n') {
            printf("CARACTER INVÁLIDO! Digite S ou N ou 0 para encerrar: ");
            scanf(" %c", &ch);
                if (ch == 'S' || ch == 's') {
                    numerado = false;
                } else if (ch == 'N' || ch == 'n' || ch == '\n') {
                    numerado = true;
                } else if (ch == '0') {
                    exit(0);
                }
        }
    }

    //TODO: marcas de stack, heap, estática e código

    if (centralizado) printf("Colunas centralizadas\n"); else printf("Margem direita ampliada\n");
    if (numerado) printf("Endereços em todas as colunas\n"); else printf("Endereços somente\n");
    //return 1;

    /*
     * Contas
     */

    /*
     * |------------------------------------LARG_A4---------------------------------------|
     * |--(1)--||-----(2)-----||-(3)-|-----(2)-----||-(3)-|  ....  |-----(2)-----||--(4)--|
     * 
     *  1 - margem_esq
     *  2 - largura
     *  3 - entre_cols
     *  4 - margem_dir
     * 
     *  LARG_A4 == margem_esq + n_cols*largura + (n_cols-1)*entre_cols + margem_dir
     *
     *  Considerando: 
     *  > margem_esq = FATOR * entre_cols
     *  > margem_dir = FATOR * entre_cols
     *
     *  entre_cols = (LARG_A4 - n_cols*largura) / (2*FATOR + n_cols - 1)
     */
    #define FATOR 1.5
    float largura = 60.0;
    float margem_esq, margem_dir, entre_cols;
    if (n_cols > 1) {
        entre_cols = (LARG_A4 - n_cols*largura) / (2*FATOR + n_cols - 1);
        margem_esq = margem_dir = FATOR*entre_cols;
    } else {
        entre_cols = 0;
        margem_esq = margem_dir = (LARG_A4 - largura) / 2;
    }

    /*
     * 
     */
    float margem_sup = 40.0;
    float altura = ALT_A4 - 2*margem_sup;
    float alt_cel = altura / n_cels;
    float alt_fonte = 10.0;
    float base = 2.0 + (alt_cel - alt_fonte) / 2;

    /*
     * Desenho
     */
    cairo_surface_t* surface =
        cairo_pdf_surface_create("mem.pdf", LARG_A4, ALT_A4);
    cairo_t* cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 0.2);
    cairo_select_font_face(cr, "courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, alt_fonte);

    int i, j; float x, y; int end_cel = end_inicial; char pad[32];
    for(i=0, x=margem_esq; i<n_cols; i++, x+=largura+entre_cols){
        cairo_rectangle(cr, x, margem_sup, largura, altura);

        for(j=0, y=margem_sup+alt_cel; j<n_cels-1; j++, y+=alt_cel) {
            cairo_move_to(cr, x, y);
            cairo_line_to(cr, x+largura, y);
            cairo_move_to(cr, x-0.5*largura,y-base);
            sprintf(pad, "%d", end_cel++);
            cairo_show_text(cr, pad);
        }
        cairo_move_to(cr, x-0.5*largura,y-base);
        sprintf(pad, "%d", end_cel++);
        cairo_show_text(cr, pad);
        
    }

    cairo_stroke(cr);

    cairo_show_page(cr);

    cairo_destroy (cr);
    cairo_surface_destroy (surface);
    return 0;
}