#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "metodosIndiretos.h"
#include "funcoesAux.h"
//-----------------------------------------------------------------------------------------
// METODOS INDIRETOS DE SOLUCAO DE SISTEMAS LINEARES
//-----------------------------------------------------------------------------------------

double *Jacobi(double ***A, double **b, int n)
{
    int max_iter = 10;
    double *result = NULL;
    result = alocaVetor(n);
    int i, j, k;
    double soma;
    double *x0 = NULL;
    double *x_atual = NULL;
    x0 = alocaVetor(n);
    x_atual = alocaVetor(n);

    double erro = 100;
    //double tol = 0.001;

    double maxX0;
    double max_Atual;
    int iter = 0;

    for (i = 0; i < n; i++)
    {
        x0[i] = 0.1;
    }

    //verificadao da diagonal dominante
    int ctAux = 0;
    double maxN;
    for (i = 0; i < n; i++)
    {
        double maxDiag;
        maxDiag = fabs((*A)[i][i]);
        for (j = 0; j < n; j++)
        {
            if (i != j)
            {
                maxN = (*A)[i][j];
                if (maxN > maxDiag)
                {
                    ctAux += 1;
                }
            }
        }
    }
    if (ctAux == 0)
    {
        printf("diagonal dominante");
    }
    else
    {
        printf("nao diagonal dominante");
    }
    while (iter < max_iter)
    {
        for (i = 0; i < n; i++)
        {
            // calcula a soma da formula de Jacobi
            soma = 0;
            for (j = 0; j < n; j++)
            {
                if (i != j)
                {
                    soma += (*A)[i][j] * x0[j];
                }
            }
            x_atual[i] = (1 / (*A)[i][i]) * ((*b)[i] - soma);
        }
        // verifica criterio de parada
        maxX0 = fabs(x0[0]);
        max_Atual = fabs(x_atual[0]);
        for (k = 0; k < n; k++)
        {
            if (fabs(x_atual[k]) > max_Atual)
            {
                max_Atual = fabs(x_atual[k]);
            }
            if (fabs(x0[k]) > maxX0)
            {
                maxX0 = fabs(x0[k]);
            }
        }
        erro = (max_Atual - maxX0) / max_Atual;

        for (i = 0; i < n; i++)
        {
            x0[i] = x_atual[i];
        }
        iter = iter + 1;
    }
    result = x_atual;
    return result;
}

double *GaussSeidel(double ***A, double **b, int n)
{
    int max_iter = 20;
    double *result = NULL;
    result = alocaVetor(n);
    int i, j, k;
    double soma, soma2;
    double *x0 = NULL;
    double *x_atual = NULL;
    x0 = alocaVetor(n);
    x_atual = alocaVetor(n);
    double erro;
    double tol = 0.000001;
    double maxX0;
    double max_Atual;
    int iter = 0;

    //inicializa vetores
    for (k = 0; k < n; k++)
    {
        x0[k] = 0;
        x_atual[k] = 0;
    }
    while (iter < max_iter)
    {
        for (i = 0; i < n; i++)
        {
            if (i == 0)
            {
                soma = 0;
                for (j = 0; j < n; j++)
                {
                    if (j != i)
                    {
                        soma = soma + (-1 * (*A)[i][j] * x0[j]);
                    }
                }
                x_atual[i] = (1 / (*A)[i][i]) * (soma + (*b)[i]);
            }
            else
            {
                soma = 0;
                for (j = 0; j < n; j++)
                {
                    if (j != i)
                    {
                        soma = soma + (-1 * (*A)[i][j] * x0[j]);
                    }
                }
                soma2 = 0;
                for (j = 0; j < i; j++)
                {
                    soma2 = soma2 + (-1 * (*A)[i][j] * x_atual[j]);
                }
                x_atual[i] = (1 / (*A)[i][i]) * (soma2 + soma + (*b)[i]);
            }
        }
        // verifica criterio de parada
        maxX0 = fabs(x0[0]);
        max_Atual = fabs(x_atual[0]);
        for (k = 0; k < n; k++)
        {
            if (fabs(x_atual[k]) > max_Atual)
            {
                max_Atual = fabs(x_atual[k]);
            }
            if (fabs(x0[k]) > maxX0)
            {
                maxX0 = fabs(x0[k]);
            }
        }
        erro = (max_Atual - maxX0) / max_Atual;
        if (erro < tol)
        {
            result = x_atual;
            return (result);
        }
        else
        {
            x0 = x_atual;
            iter += 1;
        }
    }
    return x_atual;
}
double *GradientesConjugados(double ***A, double **b, int n)
{
    double *result = NULL;

    result = alocaVetor(n);
    double *Ax = NULL;
    double *r0 = NULL;
    double *p1 = NULL;
    double *Ar0 = NULL;
    double *r1 = NULL;
    double *p_new = NULL;
    Ax = alocaVetor(n);
    r0 = alocaVetor(n);
    p1 = alocaVetor(n);
    p_new = alocaVetor(n);
    Ar0 = alocaVetor(n);
    r1 = alocaVetor(n);
    double *x0 = NULL;
    x0 = alocaVetor(n);
    double *x_atual;
    x_atual = alocaVetor(n);
    double q1 = 0.0;
    double alfa = 0.0;
    int iter = 0;
    int i_max = 100;
    double maxX0;
    double max_Atual;
    double erro = 1000;
    double soma;

    int i, j, k;
    //seguindo nomenclatura da pagina 178 do NEIDE
    //Ax0
    for (i = 0; i < n; i++)
    {
        soma = 0;
        for (j = 0; j < n; j++)
        {
            soma += (*A)[i][j] * x0[j];
        }
        Ax[i] = soma;
        r0[i] = Ax[i] + (*b)[i];
        p1[i] = -r0[i];
    }
    //A * r0
    for (i = 0; i < n; i++)
    {
        soma = 0;
        for (j = 0; j < n; j++)
        {
            soma += (*A)[i][j] * r0[j];
        }
        Ar0[i] = soma;
    }
    //calcula q1
    q1 = (dotProduct(&r0, &r0, n) / dotProduct(&Ar0, &r0, n));

    //calculo de x_atual (v k+1)
    for (i = 0; i < n; i++)
    {
        x_atual[i] = x0[i] + q1 * p1[i];
        x0[i] = x_atual[i];
    }
    //calculo de r1
    for (i = 0; i < n; i++)
    {
        soma = 0;
        for (j = 0; j < n; j++)
        {
            soma += (*A)[i][j] * p1[j];
        }
        r1[i] = r0[i] + (q1 * soma);
    }

    while (iter < i_max)
    {
        //for k>2
        alfa = (dotProduct(&r1, &r1, n) / dotProduct(&r0, &r0, n));
        //atualiza vetor r0
        for (i = 0; i < n; i++)
        {
            r0[i] = r1[i];
        }
        for (i = 0; i < n; i++)
        {
            p_new[i] = -r1[i] + (alfa * p1[i]);
            p1[i] = p_new[i];
        }
        //atualiza Ar0 como Ap_new

        for (i = 0; i < n; i++)
        {
            soma = 0;
            for (j = 0; j < n; j++)
            {
                soma += (*A)[i][j] * p_new[j];
            }
            Ar0[i] = soma;
        }
        //q1 new
        q1 = (dotProduct(&r1, &r1, n) / dotProduct(&Ar0, &p_new, n));

        //atualiza vetor resultado
        for (i = 0; i < n; i++)
        {
            x0[i] = x_atual[i];
            x_atual[i] = x0[i] + q1 * p_new[i];
        }

        // verifica criterio de parada
        maxX0 = fabs(x0[0]);
        max_Atual = fabs(x_atual[0]);
        for (k = 0; k < n; k++)
        {
            if (fabs(x_atual[k]) > max_Atual)
            {
                max_Atual = fabs(x_atual[k]);
            }
            if (fabs(x0[k]) > maxX0)
            {
                maxX0 = fabs(x0[k]);
            }
        }
        erro = (max_Atual - maxX0) / max_Atual;

        //calculo de r1
        for (i = 0; i < n; i++)
        {
            soma = 0;
            for (j = 0; j < n; j++)
            {
                soma += (*A)[i][j] * p_new[j];
            }
            r1[i] = r0[i] + q1 * soma;
        }

        iter += 1;
    }
    if (erro < 0.001)
    {
        result = x_atual;
    }
    else
    {
        result = x0;
    }
    return (result);
}
double *GradPreCondicionados(double ***A, double **b, int n)
{
    //Seguindo algoritmo em PDF

    //alocacao de memoria para os vetores
    int i, j;
    double *result;
    result = alocaVetor(n);

    //chute inicial
    double *x0 = NULL;
    x0 = alocaVetor(n);
    for (i = 0; i < n; i++)
    {
        x0[i] = 0;
    }
    //matrix diagonal
    double **B;
    B = alocaMatriz(n, n);
    for (i = 0; i < n; i++)
    {
        B[i][i] = (*A)[i][i];
    }
    double *r0 = NULL;
    r0 = alocaVetor(n);
    double *r_new = NULL;
    r_new = alocaVetor(n);
    double *z0 = NULL;
    z0 = alocaVetor(n);
    double *z_new = NULL;
    z_new = alocaVetor(n);
    double *v0 = NULL;
    v0 = alocaVetor(n);
    double *v_new = NULL;
    v_new = alocaVetor(n);
    double *Av0 = NULL;
    Av0 = alocaVetor(n);
    double *Ax0_aux = NULL;
    Ax0_aux = alocaVetor(n);

    double *x_new = NULL;
    x_new = alocaVetor(n);

    double alfa = 0.0;
    double beta = 0.0;

    int iter = 0;
    int iter_max = 20;
    double soma = 0.0;
    double soma_x0 = 0.0;
    //init
    for (i = 0; i < n; i++)
    {
        soma = 0.0;
        soma_x0 = 0.0;
        for (j = 0; j < n; j++)
        {
            soma += (*A)[i][j] * v0[j];
            soma_x0 += (*A)[i][j] * x0[j];
        }
        Av0[i] = soma;
        Ax0_aux[i] = soma_x0;
    }
    
    for (i = 0; i < n; i++)
    {
        r0[i] = (*b)[i] - Ax0_aux[i];
    }
    for (i = 0; i < n; i++)
    {
        soma = 0;
        for (j = 0; j < n; j++)
        {
            soma += B[i][j] * r0[j];
        }
        z0[i] = soma;
        v0[i] = z0[i];

    }
    //processo iterativo
    while (iter <= iter_max)
    {
        //alfa k
        alfa = dotProduct(&r0, &z0, n) / dotProduct(&Av0, &v0, n);
        //printf("iter: %d, alfa: %f\n", iter, alfa);
        //atualiza x, r
        for (i = 0; i < n; i++)
        {
            x_new[i] = x0[i] + (alfa * v0[i]);
            x0[i] = x_new[i];
            r_new[i] = r0[i] - (alfa * Av0[i]);
        }
        //atualiza z
        for (i = 0; i < n; i++)
        {
            soma = 0;
            for (j = 0; j < n; j++)
            {
                soma = B[i][j] * r_new[j];
            }
            z_new[i] = soma;
        }
        //beta
        beta = dotProduct(&r_new, &z_new, n) / dotProduct(&r0, &z0, n);
        //atualiza v
        for (i = 0; i < n; i++)
        {
            v_new[i] = z_new[i] + (beta * v0[i]);
        }
        for (i = 0; i < n; i++)
        {
            r0[i] = r_new[i];
            z0[i] = z_new[i];
            v0[i] = v_new[i];
            //x0[i] = x_new[i];
        }
        for (i = 0; i < n; i++)
        {
            soma = 0;
            for (j = 0; j < n; j++)
            {
                soma += (*A)[i][j] * v0[j];
            }
            Av0[i] = soma;
        }
        iter += 1;
    }

    return (x0);
}