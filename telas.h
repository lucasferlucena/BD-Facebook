#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <string>
#include <stdlib.h>

using namespace std;

void telaComentario(){
    system("cls");
    cout << "Selecione uma das opções" << endl << endl;
    cout << "1. Ver repostas" << endl;
    cout << "2. Responder" << endl;

    int option;
    cin >> option;
    system("cls");
    switch(option){
        case 1:
            verRespostas();
            telaComentario();
            break;
        case 2:
            criarResposta();
            telaComentario();
            break;
        default:
            break;
    }
}
void telaPublicacao(){
    system("cls");
    cout << "Selecione uma das opções" << endl << endl;
    cout << "1. Ver comentários" << endl;
    cout << "2. Comentar" << endl;

    int option;
    cin >> option;
    system("cls");
    switch(option){
        case 1:
            if(verComentarios()){
                telaComentario();
            }
            telaPublicacao();
            break;
        case 2:
            criarComentario();
            telaPublicacao();
            break;
        default:
            if(verPublicacoes()){
                telaPublicacao();
            }
            break;
    }
}

void telaUserVisitado(){
    system("cls");
    cout << "Selecione uma das opções" << endl << endl;
    if(amizadeVerificador() == 1){
        cout << "1. Ver Perfil" << endl;
        cout << "2. Publicar" << endl;
        cout << "3. Ver Publicacoes do Mural" << endl;
        cout << "4. Ver Amigos" << endl;
        cout << "5. Ver Amigos em Comum" << endl;
        cout << "6. Desfazer Amizade" << endl;
    }
    if(amizadeVerificador() == 5){
        return;
    }
    if(amizadeVerificador() == -1){
        cout << "1. Ver Perfil" << endl;
        cout << "2. Solicitar Amizade" << endl;
    }
    else{
        cout << "1. Ver Perfil" << endl;
    }
    int option;
    cin >> option;
    system("cls");
    switch(option){
        case 1:
            verPerfil();
            telaUserVisitado();
            break;
        case 2:
            if(amizadeVerificador() == 1)
                criarPublicacao();
            if(amizadeVerificador() == -1)
                solicitarAmizade();
            telaUserVisitado();
            break;
        case 3:

            if(amizadeVerificador() == 1 && verPublicacoes()){
                telaPublicacao();
            }
            telaUserVisitado();
            break;
        case 4:
            //TODO
            break;
        case 5:
            //TODO
            break;
        case 6:
            //TODO
            break;
        default:

            break;
    }
}

void telaPrincipal(){
    system("cls");
    cout << "Selecione uma das opções" << endl << endl;
    cout << "1. Publicar" << endl;
    cout << "2. Ver Publicacoes do Mural" << endl;
    cout << "3. Buscar Pessoas" << endl;
    cout << "4. Buscar Grupos" << endl;
    cout << "5. Criar Grupo" << endl;
    cout << "6. Ver Amigos" << endl;
    cout << "7. Editar Opções de Visibilidade" << endl;
    cout << "8. Solicitaçoes Recebidas" << endl;

    int option;
    cin >> option;
    system("cls");
    switch(option){
        case 1:
            criarPublicacao();
            telaPrincipal();
            break;
        case 2:
            if(verPublicacoes()){
                telaPublicacao();
            }
            telaPrincipal();
            break;
        case 3:
            if(buscarPessoas())
                telaUserVisitado();
            telaPrincipal();
            break;
        case 4:
            buscarGrupos();
            telaPrincipal();
            break;
        case 5:
            criarGrupo();
            telaPrincipal();
            break;
        case 6:
            if(verAmigos())
                telaUserVisitado();
            telaPrincipal();
            break;
        case 7:
            //amizadeVerificador();
            break;
        case 8:
            verSolicitacoes();
            telaPrincipal();
            break;
        default:
            break;
    }
}

int telaInicial(){
    cout << "Bem vindo ao Facebook" << endl;
    cout << "Selecione uma das opções" << endl << endl;
    cout << "1. Efetuar Login" << endl;
    cout << "2. Cadastrar-se" << endl;

    int option;
    cin >> option;
    system("cls");
    switch(option){
        case 1:
            if(efetuarLogin())
                telaPrincipal();
            else{
                system("cls");
                telaInicial();
            }
            telaInicial();
            break;
        case 2:
            cadastrarUsuario();
            telaPrincipal();
            telaInicial();
            break;
        default:
            break;
    }
    return option;
}



