#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <string>
#include <stdlib.h>

using namespace std;

void telaResposta(){
     system("cls");
    cout << "Selecione uma das opções" << endl << endl;
    if(donoConteudo(2) || donoMural(2) || donoGrupo(2))
        cout << "1. Excluir Resposta" << endl;
    cout << "0. Voltar" << endl;

    int option;
    cin >> option;
    system("cls");

    switch(option){
        case 1:
            excluirResposta();
            telaResposta();
            break;
    }
}


void telaComentario(){
    system("cls");
    cout << "Selecione uma das opções" << endl << endl;
    cout << "1. Ver repostas" << endl;
    cout << "2. Responder" << endl;
    if(donoConteudo(1) || donoMural(1) || donoGrupo(1))
        cout << "3. Excluir Comentario" << endl;
    cout << "0. Voltar" << endl;

    int option;
    cin >> option;
    system("cls");
    switch(option){
        case 1:
            if(verRespostas())
                telaResposta();
            telaComentario();
            break;
        case 2:
            criarResposta();
            telaComentario();
            break;
        case 3:
            if(donoConteudo(1) || donoMural(1) || donoGrupo(1))
                excluirComentario();
        default:
            break;
    }
}
void telaPublicacao(){
    system("cls");
    cout << "Selecione uma das opções" << endl << endl;
    cout << "1. Ver comentários" << endl;
    cout << "2. Comentar" << endl;
    if(donoConteudo(0) || donoMural(0) || donoGrupo(0))
        cout << "3. Excluir Publicacao" << endl;
    cout << "0. Voltar" << endl;

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
        case 3:
            if(donoConteudo(0) || donoMural(0) || donoGrupo(0))
                excluirPostagem();
        default:
            if(verPublicacoes()){
                telaPublicacao();
            }
            break;
    }
}

void telaUserVisitado(){
    system("cls");

    nomeUsuario();

    int visibilidade;

    cout << "Selecione uma das opções" << endl << endl;

    switch(checarVisibilidade()){
    case 0:
        if(amizadeVerificador() == 1){
            visibilidade = 1;
        }else{
            visibilidade = 0;
        }
        break;
    case 1:
        if(amigoamigoVerificador() || amizadeVerificador() == 1){
            visibilidade = 1;
        }else{
            visibilidade = 0;
        }
        break;
    case 2:
        visibilidade = 1;
        break;
    }
    if(visibilidade){
        cout << "1. Ver Perfil" << endl;
        cout << "2. Publicar" << endl;
        cout << "3. Ver Publicacoes do Mural" << endl;
        cout << "4. Ver Amigos" << endl;
        cout << "5. Ver Amigos em Comum" << endl;
        cout << "6. Desfazer Amizade" << endl;
    }else{
        if(amizadeVerificador() == -2){
        return;
        }
        if(amizadeVerificador() == -1){
            cout << "1. Solicitar Amizade" << endl;
        }
        if(amizadeVerificador() == 0){
            cout << "Pedido de Amizade Solicitado" << endl;
        }
    }
    cout << "7. Bloquear Usuario" << endl;
    cout << "0. Voltar" << endl;
    int option;
    cin >> option;
    system("cls");
    switch(option){
        case 1:
            if(visibilidade)
                verPerfil();
            else{
            if(amizadeVerificador() == -1)
                solicitarAmizade();
            }
            telaUserVisitado();
            break;
        case 2:
            if(visibilidade)
                criarPublicacao();
            telaUserVisitado();
            break;
        case 3:
            if(visibilidade && verPublicacoes()){
                telaPublicacao();
            }
            telaUserVisitado();
            break;
        case 4:
            if(visibilidade){
                if(verAmigos())
                    telaUserVisitado();
            }
            telaUserVisitado();
            break;
        case 5:
            if(visibilidade)
                verAmigosEmComum();
            telaUserVisitado();
            break;
        case 6:
            desfazerAmizade();
            telaUserVisitado();
            break;
        case 7:
            bloquearUsr();
            break;
        default:
            break;
    }
}


void telaGrpVisitado(){
    system("cls");

    nomeGrupo();
    int aux;
    cout << "Selecione uma das opções" << endl << endl;

    if(checarVisibilidadeGrp() == 0){
        if(membroVerificador() == -1){
            cout << "1. Solicitar Entrada no Grupo" << endl;
            aux=0;
        }
        if(membroVerificador() == 0){
            aux=1;
        }
        if(membroVerificador() == 1){
            cout << "1. Ver Publicacoes do Mural" << endl;
            cout << "2. Ver Membros" << endl;
            cout << "3. Publicar" << endl;
            cout << "4. Sair do Grupo" << endl;
            aux=2;
        }
        if(membroVerificador() == 2){
            cout << "1. Ver Publicacoes do Mural" << endl;
            cout << "2. Ver Membros" << endl;
            cout << "3. Publicar" << endl;
            cout << "4. Sair do Grupo" << endl;
            cout << "5. Ver Solicitacoes de Entrada no Grupo" << endl;
            cout << "6. Editar Grupo" << endl;
            cout << "7. Bloqueados do Grupo" << endl;
            aux=3;
        }
    }else{
            cout << "1. Ver Publicacoes do Mural" << endl;
            cout << "2. Ver Membros" << endl;
        if(membroVerificador() == -1){
            cout << "3. Solicitar Entrada no Grupo" << endl;
            aux=4;
        }
        if(membroVerificador() == 0){
            //nada
            aux=5;
        }
        if(membroVerificador() == 1){
            cout << "3. Publicar" << endl;
            cout << "4. Sair do Grupo" << endl;
            aux=2;
        }
        if(membroVerificador() == 2){
            cout << "3. Publicar" << endl;
            cout << "4. Sair do Grupo" << endl;
            cout << "5. Ver Solicitacoes de Entrada no Grupo" << endl;
            cout << "6. Editar Grupo" << endl;
            cout << "7. Bloqueados do Grupo" << endl;
            aux=3;
        }
    }
    cout << "0. Voltar" << endl;
    int option;
    cin >> option;
    system("cls");
    switch(option){
        case 1:
            if(aux == 1){
                //nada
            }else{
                if(aux == 0){
                    solicitarAssociacao();
                }
                else{
                    if(verPublicacoes())
                        telaPublicacao();
                }
            }
            telaGrpVisitado();
            break;
        case 2:
           if(aux == 0 || aux == 1){

           }else{
                if(aux == 3){
                    int x = verMembrosADM();
                    if(x == -1)
                        break;
                    if(x == 1)
                        telaUserVisitado();
               }else{
                verMembros();
               }
           }
            break;
        case 3:
           if(aux == 0 || aux == 1 || aux == 5){

           }else{
                if(aux == 4){
                    solicitarAssociacao();
                }else{
                    criarPublicacao();
                }
            }
            telaGrpVisitado();
            break;
        case 4:
            if(aux == 0 || aux == 1 || aux == 4 || aux == 5){
            }else{
                desfazerAssociacao();
            }
            telaGrpVisitado();
            break;
        case 5:
            if(aux == 3){
                verSolicitacoesGrp();
            }
            telaGrpVisitado();
            break;
        case 6:
            if(aux == 3){
                editarGrupo();
            }
            telaGrpVisitado();
            break;
        case 7:
            if(aux == 3){
                bloqueadosGrupo();
            }
            telaGrpVisitado();
        default:
            break;
    }
}


void telaPrincipal(){
    id_mural_vis = id_mural;
    id_usr_vis = id_usr;

    system("cls");

    nomeUsuario();

    cout << "Selecione uma das opções" << endl << endl;
    cout << "1. Publicar" << endl;
    cout << "2. Ver Publicacoes do Mural" << endl;
    cout << "3. Buscar Pessoas" << endl;
    cout << "4. Buscar Grupos" << endl;
    cout << "5. Criar Grupo" << endl;
    cout << "6. Ver Amigos" << endl;
    cout << "7. Editar Perfil" << endl;
    cout << "8. Solicitaçoes Recebidas" << endl;
    cout << "9. Ver Perfil" << endl;
    cout << "10. Ver Bloqueados" << endl;
    cout << "0. Sair" << endl;

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
            if(buscarGrupos())
                telaGrpVisitado();
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
            editarPerfil();
            telaPrincipal();
            break;
        case 8:
            verSolicitacoes();
            telaPrincipal();
            break;
        case 9:
            verPerfil();
            telaPrincipal();
        case 10:
            verBloqueados();
            telaPrincipal();
        default:
            break;
    }
}

int telaInicial(){
    cout << "Bem vindo ao Facebook" << endl;
    cout << "Selecione uma das opções" << endl << endl;
    cout << "1. Efetuar Login" << endl;
    cout << "2. Cadastrar-se" << endl;
    cout << "0. Finalizar" << endl;

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
    return 0;
}



