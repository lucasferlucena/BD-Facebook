#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;

MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;
MYSQL_ROW row1;
MYSQL_RES* res1;
MYSQL_ROW row2;
MYSQL_RES* res2;
int qstate;

string id_usr = "";

string id_usr_vis = "";
string id_grp_vis = "";

string id_pub_vis = "";
string id_cmnt_vis = "";
string id_resp_vis = "";

string id_mural = "";
string id_mural_vis = "";

void executeQuery(string query){
     const char* q = query.c_str();
     qstate = mysql_query(conn,q);
}

////////////////////    BLOQUEIO - APAGAR COISAS

void excluirRespostaBloq(){
    executeQuery("delete from resposta where cod_wall = '"+id_mural_vis+"' and cod_usr = '"+id_usr_vis+"'");
}

void excluirComentarioBloq(){
    executeQuery("select * from comentario where cod_wall = '"+id_mural_vis+"' and cod_usr = '"+id_usr_vis+"'");
    res = mysql_store_result(conn);

    vector<string> idComentarios;

    while(row = mysql_fetch_row(res)){
        //if(!row)
        //TODO
        // verificar se existe comentarios a serem apagas
        idComentarios.push_back(row[0]);
    }

    vector<string> idRespostas[idComentarios.size()];

    for(int i = 0; i < idComentarios.size(); i++){
        executeQuery("select * from resposta where cod_coment = '"+idComentarios[i]+"'");
        res = mysql_store_result(conn);

        while(row = mysql_fetch_row(res))
            idRespostas[i].push_back(row[0]);

        for(int j = 0; j < idRespostas[i].size(); j++)
            executeQuery("delete from resposta where cod_resp = '"+idRespostas[i][j]+"'");

        executeQuery("delete from comentario where cod_coment = '"+idComentarios[i]+"'");
    }
}

void excluirPostagemBloq(){
    executeQuery("select * from postagem where cod_wall = '"+id_mural_vis+"' and cod_usr = '"+id_usr_vis+"'");
    res = mysql_store_result(conn);

    vector<string> idPostagem;

    while(row = mysql_fetch_row(res))
        idPostagem.push_back(row[0]);

    for(int k = 0; k < idPostagem.size(); k++){
        executeQuery("select * from comentario where cod_post = '"+idPostagem[k]+"'");
        res = mysql_store_result(conn);

        vector<string> idComentarios;

        while(row = mysql_fetch_row(res))
            idComentarios.push_back(row[0]);

        vector<string> idRespostas[idComentarios.size()];

        for(int i = 0; i < idComentarios.size(); i++){
            executeQuery("select * from resposta where cod_coment = '"+idComentarios[i]+"'");
            res = mysql_store_result(conn);

            while(row = mysql_fetch_row(res))
                idRespostas[i].push_back(row[0]);

            for(int j = 0; j < idRespostas[i].size(); j++)
                executeQuery("delete from resposta where cod_resp = '"+idRespostas[i][j]+"'");

            executeQuery("delete from comentario where cod_resp = '"+idComentarios[i]+"'");
        }
        executeQuery("delete from postagem where cod_post = '"+idPostagem[k]+"'");
    }
}

////////////////////
/////////////////// CADASTRAR (PESSOAS E GRUPOS)
int cadastrarUsuario(){
    system("cls");
    string nome, sobrenome, email, senha, cidade, foto, cod_wall, visibilidade;

    if(conn){
        cout << "Digite seu nome: "<< endl;
        cin >> nome;
        cin.ignore(80, '\n');
        cout << "Digite seu sobrenome: "<< endl;
        getline(cin, sobrenome);
        cout << "Digite seu email: "<< endl;
        cin >> email;
        cout << "Digite sua senha: "<< endl;
        cin >> senha;
        cin.ignore(80, '\n');
        cout << "Digite sua cidade: "<< endl;
        getline(cin, cidade);
        cout << "Registre sua foto: "<< endl;
        cin >> foto;
        cout << "Qual a visibilidade do seu Mural: " << endl;
        cout << "0. Amigos" << endl;
        cout << "1. Amigos de Amigos" << endl;
        cout << "2. Publico" << endl;
        cin >> visibilidade;

        if( visibilidade != "2" && visibilidade != "0" && visibilidade != "1")
            visibilidade = "0";

        executeQuery("insert into mural() values()");

        qstate = mysql_query(conn ,"SELECT LAST_INSERT_ID()");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        cod_wall = row[0];
        executeQuery("insert into usuario(nome,sobrenome,email,senha,cidade,foto,visibilidade,cod_wall) values('"+nome+"','"+sobrenome+"','"+email+"','"+senha+"','"+cidade+"','"+foto+"','"+visibilidade+"','"+cod_wall+"')");

        qstate = mysql_query(conn ,"SELECT LAST_INSERT_ID()");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        id_usr = row[0];
        id_usr_vis = id_usr;
        id_mural = cod_wall;
        id_mural_vis = id_mural;

        if(!qstate)
            cout<<"record inserted successfully..."<<endl;
        else
            cout<<"query problem: "<<mysql_error(conn)<<endl;
    }
    else
    {
        cout<<"connection problem: "<<mysql_error(conn)<<endl;
    }
}

int criarGrupo(){
    system("cls");
    string nome, descricao, foto, cod_wall, visibilidade;

    if(conn){
        cout << "Digite o nome do grupo: "<< endl;
        cin >> nome;
        cin.ignore(80, '\n');
        cout << "Digite a descricao: "<< endl;
        getline(cin, descricao);
        cout << "Registre sua foto: "<< endl;
        cin >> foto;
        cout << "Qual a visibilidade do seu Mural: " << endl;
        cout << "0. Privado" << endl;
        cout << "1. Publico" << endl;
        cin >> visibilidade;

        if( visibilidade != "1" && visibilidade != "0")
            visibilidade = "0";

        executeQuery("insert into mural() values()");

        qstate = mysql_query(conn ,"SELECT LAST_INSERT_ID()");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        cod_wall = row[0];

        executeQuery("insert into grupo(nome,descricao,foto,visibilidade,cod_wall) values('"+nome+"','"+descricao+"','"+foto+"','"+visibilidade+"','"+cod_wall+"')");

        if(!qstate)
            cout<<"Grupo "<< nome <<" criado com sucesso"<<endl;
        else
            cout<<"query problem: "<<mysql_error(conn)<<endl;


        qstate = mysql_query(conn ,"SELECT LAST_INSERT_ID()");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        string id_grupo = row[0];

        executeQuery("insert into associacao(cod_grp, cod_usr, condicao) values('"+id_grupo+"','"+id_usr+"','"+"2"+"')");
    }
    else
    {
        cout<<"connection problem: "<<mysql_error(conn)<<endl;
    }
}
///////////////////////
/////////////////////// EFETUAR LOGIN
int efetuarLogin(){
    system("cls");
    while(1){
    system("cls");
    string email, senha;

    cout << "Digite seu email: "<< endl;
    cin >> email;

    executeQuery("select * from usuario where email = '"+email+"'");

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    if(!row){
        int choice;
        cout << "email inexistente"<<endl;
        cout << "digite 0 para sair, 1 para continuar tentando" << endl;
        cin >> choice;
        if(!choice)
            return 0;
        continue;
    }
    string senha_real = row[7];
    cout << "Digite sua senha: "<<endl;
    cin >> senha;

    if(senha_real == senha){
        cout << "Login efetuado com sucesso"<< endl;
        id_usr = row[0];
        id_usr_vis = id_usr;
        id_mural = row[8];
        id_mural_vis = id_mural;
        return 1;
    }
    else
        cout <<"Acesso negado!"<<endl;
        return 0;
    }
}
///////////////////////
/////////////////////// VERIFICADORES DE AMIZADE OU ASSOCIACAO
int amizadeVerificador(){
    executeQuery("select * from amizade where (cod_solicitador ='"+id_usr+"' and cod_solicitado = '"+id_usr_vis+"') or (cod_solicitador ='"+id_usr_vis+"' and cod_solicitado = '"+id_usr+"')");
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    if(id_usr_vis == id_usr)
        return -2;
    if(!row)
        return -1;
    return atoi(row[2]);
}

int amigoamigoVerificador(){
    executeQuery("select * from amizade where (cod_solicitado = '"+id_usr_vis+"' or cod_solicitador = '"+id_usr_vis+"') and condicao = '"+"1"+"'");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        string id;

        if(id_usr_vis == row[1])
            id = row[0];
        else
            id = row[1];

        executeQuery("select * from amizade where (cod_solicitador ='"+id_usr+"' and cod_solicitado = '"+id+"') or (cod_solicitador ='"+id+"' and cod_solicitado = '"+id_usr+"')");

        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        if(!row1)
            continue;
        return atoi(row1[2]);
        i++;
    }
    return 0;
}

int membroVerificador(){
   executeQuery("select * from associacao where (cod_grp = '"+id_grp_vis+"' and cod_usr = '"+id_usr+"')");
    res2 = mysql_store_result(conn);
    row2 = mysql_fetch_row(res2);

    if(!row2)
        return -1;

    return atoi(row2[2]);
}

int verificarBloqueio(){
    executeQuery("select * from bloqueio where (cod_bloqueador = '"+id_usr_vis+"' and cod_bloqueado = '"+id_usr+"') or (cod_bloqueador = '"+id_usr+"' and cod_bloqueado = '"+id_usr_vis+"')");
    res2 = mysql_store_result(conn);
    row2 = mysql_fetch_row(res2);

    if(!row2)
        return 0;

    return 1;
}
/////////////////////////////
////////////////////// BUSCAS
int buscarPessoas(){
    system("cls");

    executeQuery("select * from usuario");

    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        id_usr_vis = row[0];
        if(!verificarBloqueio()){
            if(!i)
                cout<< "RESULTADOS:" << endl;

            cout << ++i << ": " << row[2] << " " << row[3] << endl;
        }
    }

    if(!row&&i==0){
        cout << "Nenhum resultado encontrado" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para visitar um grupo: " << endl;
    cin >> reg;

    if(!reg)
        return 0;

    executeQuery("select * from usuario");

    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        id_usr_vis = row[0];
        if(!verificarBloqueio()){
            if(++i == reg){
                id_usr_vis = row[0];
                id_mural_vis = row[8];
                return 1;
            }
        }
    }

    return 0;
}

int buscarGrupos(){
    system("cls");

    executeQuery("select * from grupo");

    res = mysql_store_result(conn);
    int i = 0;
    string aux_grp = id_grp_vis;
    string aux_id = id_usr_vis;
    while(row=mysql_fetch_row(res)){
        id_grp_vis = row[0];
        if(membroVerificador() < 3){
            if(!i)
                cout<< "RESULTADOS:" << endl;
            cout << ++i << ": " << row[1] << endl;
        }
    }

    if(!row&&i==0){
        cout << "Nenhum resultado encontrado" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para visitar um grupo: " << endl;
    cin >> reg;

    if(!reg)
        return 0;

    executeQuery("select * from grupo");

    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        id_grp_vis = row[0];
        if(membroVerificador() < 3){
            if(++i == reg){
                id_grp_vis = row[0];
                id_mural_vis = row[5];
                return 1;
            }
        }
    }
    return 0;
}
/////////////////////////
///////////////////////// CRIAR PUBLICAÇÕES, COMENTÁRIOS E RESPOSTAS
int criarPublicacao(){
    system("cls");
    string foto="", texto="", id_wall;
    int option;

    cout << "Oque voce deseja punlicar?" << endl << endl;
    cout << "1. Foto" << endl;
    cout << "2. Texto" << endl;
    cout << "3. Foto e Texto" << endl;
    cin.ignore(80, '\n');
    cin >> option;
    cin.ignore(80, '\n');
    system("cls");

    switch(option){
        case 1:
            cout << "Insira sua foto aqui: " << endl;
            cin >> foto;
            if(foto == ""){
                cout << "Publicacao Invalida";
                return 0;//retornar para tela anterior
            }
            break;
        case 2:
            cout << "Digite oque voce quer publicar: " << endl;
            getline(cin, texto);
            if(texto == ""){
                cout << "Publicacao Invalida";
                return 0;//retornar para tela anterior
            }
            break;
        case 3:
            cout << "Digite oque voce quer publicar: " << endl;
            getline(cin, texto);
            cin.ignore(80, '\n');
            cout << "Insira a sua foto" << endl;
            cin >> foto;
            if(texto == "" && foto == ""){
                cout << "Publicacao Invalida";
                return 0;//retornar para tela anterior
            }
            break;
        default:
            cout << "Comando Invalido" << endl;
            return 0;//retornar para tela anterior
    }

    executeQuery("select cod_wall from usuario where cod_usr = '"+id_usr_vis+"'");

    res = mysql_store_result(conn);
    row=mysql_fetch_row(res);
    id_wall = row[0];

    executeQuery("insert into postagem(texto, foto, cod_wall, cod_usr) values('"+texto+"','"+foto+"','"+id_mural_vis+"','"+id_usr+"')");

    cout << "Postagem Publicada" << endl;
    return 1;
}

int criarComentario(){
    system("cls");
    cin.ignore(80, '\n');
    string texto="", id_wall;

    cout << "Digite o seu comentario: " << endl;
    getline(cin, texto);
    if(texto == ""){
        cout << "Comentario Invalido";
        return 0;//retornar para tela anterior
    }

    executeQuery("select cod_wall from usuario where cod_usr = '"+id_usr_vis+"'");

    res = mysql_store_result(conn);
    row=mysql_fetch_row(res);
    id_wall = row[0];

    executeQuery("insert into comentario(texto, cod_wall, cod_usr, cod_post) values('"+texto+"','"+id_mural_vis+"','"+id_usr+"','"+id_pub_vis+"')");

    cout << "Comentario Publicado" << endl;
}

int criarResposta(){
    system("cls");
    cin.ignore(80, '\n');
    string texto="", id_wall;

    cout << "Digite a sua resposta: " << endl;
    getline(cin, texto);
    if(texto == ""){
        cout << "Resposta Invalido";
        return 0;//retornar para tela anterior
    }

    executeQuery("select cod_wall from usuario where cod_usr = '"+id_usr_vis+"'");

    res = mysql_store_result(conn);
    row=mysql_fetch_row(res);
    id_wall = row[0];

    executeQuery("insert into resposta(texto, cod_wall, cod_usr, cod_post, cod_coment) values('"+texto+"','"+id_mural_vis+"','"+id_usr+"','"+id_pub_vis+"','"+id_cmnt_vis+"')");

    return 1;
}
///////////////////////
/////////////////////// VISUALIZAR PUBLICAÇÕES, COMENTARIOS E RESPOSTAS
int verPublicacoes(){
    system("cls");
    string texto = "", foto = "", nome_usr_pub = "", sobrenome_usr_pub = "", id_wall, nome_usr_mural, sobrenome_usr_mural;

    executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
    res = mysql_store_result(conn);
    row=mysql_fetch_row(res);

    id_wall = row[8];
    nome_usr_mural = row[2];
    sobrenome_usr_mural = row[3];

    executeQuery("select * from postagem where cod_wall = '"+id_mural_vis+"'");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
       // if(!i)
            //cout<< "PUBLICACOES NO MURAL DE "<< nome_usr_mural << " "<< sobrenome_usr_mural <<":" << endl << endl;
        string id = row[4];
        executeQuery("select * from usuario where cod_usr = '"+ id +"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        nome_usr_pub = row1[2];
        sobrenome_usr_pub = row1[3];

        id_usr_vis = id;
        if(!verificarBloqueio()){
            cout << "--------------------------------------------------------" << endl;
            cout << "#Publicacao" << ++i << endl << endl;
            cout << nome_usr_pub << " " << sobrenome_usr_pub << endl;
            if(row[2] != "")
                cout << row[2] << endl;
            if(row[1] != "")
                cout << row[1] << endl;
            cout << "--------------------------------------------------------" << endl;
        }
    }

    if(!row&&i==0){
        cout << "Nenhuma Publicacao" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para ver as opcoes da postagem correspondente: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

    executeQuery("select * from postagem where cod_wall = '"+id_mural_vis+"'");
    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        id_usr_vis = row[4];
        if(!verificarBloqueio()){
            if(++i == reg){
                id_pub_vis = row[0];
                return 1;
            }
        }
    }

    return 0;
}

int verComentarios(){
    system("cls");
    string texto = "", nome_usr_pub = "", sobrenome_usr_pub = "", id_wall, nome_usr_comment, sobrenome_usr_comment;

   /* executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
    res = mysql_store_result(conn);
    row=mysql_fetch_row(res);
    id_wall = row[8];
    nome_usr_mural = row[2];
    sobrenome_usr_mural = row[3];
*/
    executeQuery("select * from comentario where cod_post = '"+id_pub_vis+"'");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "COMENTARIOS" << endl << endl;
        string id = row[3];
        executeQuery("select * from usuario where cod_usr = '"+id+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        nome_usr_comment = row1[2];
        sobrenome_usr_comment = row1[3];

        id_usr_vis = id;
        if(!verificarBloqueio()){
            cout << "--------------------------------------------------------" << endl;
            cout << "#Comentario" << ++i << endl << endl;
            cout << nome_usr_comment << " " << sobrenome_usr_comment << endl;
            cout << row[1] << endl;
            cout << "--------------------------------------------------------" << endl;
        }
    }

    if(!row&&i==0){
        cout << "Nenhum Comentario" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para ver as opcoes do comentario correspondente: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

    executeQuery("select * from comentario where cod_post = '"+id_pub_vis+"'");
    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        id_usr_vis = row[3];
        if(!verificarBloqueio()){
            if(++i == reg){
                id_cmnt_vis = row[0];
                return 1;
            }
        }
    }

    return 0;
}

int verRespostas(){
    system("cls");
    string texto = "", nome_usr_pub = "", sobrenome_usr_pub = "", id_wall, nome_usr_reply, sobrenome_usr_reply;

   /* executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
    res = mysql_store_result(conn);
    row=mysql_fetch_row(res);
    id_wall = row[8];
    nome_usr_mural = row[2];
    sobrenome_usr_mural = row[3];
*/
    executeQuery("select * from resposta where cod_coment = '"+id_cmnt_vis+"'");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "RESPOSTAS" << endl << endl;
        string id = row[3];
        executeQuery("select * from usuario where cod_usr = '"+id+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        nome_usr_reply = row1[2];
        sobrenome_usr_reply = row1[3];

        id_usr_vis = id;
        if(!verificarBloqueio()){
            cout << "--------------------------------------------------------" << endl;
            cout << "#Resposta" << ++i << endl << endl;
            cout << nome_usr_reply << " " << sobrenome_usr_reply << endl;
            cout << row[1] << endl;
            cout << "--------------------------------------------------------" << endl;
        }
    }

    if(!row&&i==0){
        cout << "Nenhum Comentario" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar" << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

    executeQuery("select * from resposta where cod_coment = '"+id_cmnt_vis+"'");
    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        id_usr_vis = row[3];
        if(!verificarBloqueio()){
            if(++i == reg){
                id_resp_vis = row[0];
                return 1;
            }
        }
    }

    return 0;
}
///////////////////////
/////////////////////////////
int verPerfil(){
    executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    cout << "FOTO" << endl;
    cout << row[4] << endl;
    cout << "Nome: " << row[2] << " " << row[3] << endl;

    int i;

    cin >> i;
    return 0;
}
///////////////////////////////
/////////////////////////////// VISUALIZAR SOLICITAÇÕES (USUARIO OU GRUPO)
int verSolicitacoes(){
    executeQuery("select * from amizade where cod_solicitado = '"+id_usr+"' and condicao = '"+"0"+"'");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "SOLICITACOES" << endl << endl;

        string id = row[0];
        executeQuery("select * from usuario where cod_usr = '"+id+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        cout << "--------------------------------------------------------" << endl;
        cout << ++i <<". "<< row1[2] << " " << row1[3] << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    if(!row&&i==0){
        cout << "Nenhuma Solicitacao" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para aceitar ou nao a solicitacao correspondente: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

    executeQuery("select * from amizade where cod_solicitado = '"+id_usr+"' and condicao = '"+"0"+"'");
    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            system("cls");

            string solicitador = row[0], solicitado = row[1];
            int opt;
            cout << "1. aceitar         2.recusar"<< endl;
            cin >> opt;

            if(opt == 1)
                executeQuery("update amizade set condicao = 1 where cod_solicitador = '"+solicitador+"' and cod_solicitado = '"+solicitado+"'");
            else
                executeQuery("delete from amizade where cod_solicitador = '"+solicitador+"' and cod_solicitado = '"+solicitado+"'");
        }
    }

    return 1;
}

int verSolicitacoesGrp(){
    executeQuery("select * from associacao where cod_grp = '"+id_grp_vis+"' and condicao = '"+"0"+"'");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "SOLICITACOES" << endl << endl;

        string id = row[1];

        executeQuery("select * from usuario where cod_usr = '"+id+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        cout << "--------------------------------------------------------" << endl;
        cout << ++i <<". "<< row1[2] << " " << row1[3] << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    if(!row&&i==0){
        cout << "Nenhuma Solicitacao" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para aceitar ou nao a solicitacao correspondente: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

    executeQuery("select * from associacao where cod_grp = '"+id_grp_vis+"' and condicao = '"+"0"+"'");
    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            system("cls");

            string solicitador = row[1];
            int opt;
            cout << "1. aceitar         2.recusar"<< endl;
            cin >> opt;

            if(opt == 1)
                executeQuery("update associacao set condicao = 1 where cod_grp = '"+id_grp_vis+"' and cod_usr = '"+solicitador+"'");
            else
                executeQuery("delete from associacao where cod_grp = '"+id_grp_vis+"' and cod_usr = '"+solicitador+"'");
        }
    }

    return 1;
}
/////////////////////////////
//////////////////////////// FAZER SOLICITAÇÕES

void solicitarAmizade(){
    executeQuery("insert into amizade(cod_solicitador,cod_solicitado,condicao) values('"+id_usr+"','"+id_usr_vis+"','"+"0"+"')");
}

void solicitarAssociacao(){
    executeQuery("insert into associacao(cod_grp,cod_usr,condicao) values('"+id_grp_vis+"','"+id_usr+"','"+"0"+"')");
}
///////////////////////////
/////////////////////////// VISUALIZAR AMIGOS OU MEMBROS OU BLOQUEADOS
int verAmigos(){
    executeQuery("select * from amizade where (cod_solicitado = '"+id_usr_vis+"' or cod_solicitador = '"+id_usr_vis+"') and condicao = '"+"1"+"'");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "AMIZADES" << endl << endl;

        string id;

        if(id_usr_vis == row[1])
            id = row[0];
        else
            id = row[1];

        executeQuery("select * from usuario where cod_usr = '"+id+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        cout << "--------------------------------------------------------" << endl;
        cout << ++i <<". "<< row1[2] << " " << row1[3] << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    if(!row&&i==0){
        cout << "Nenhum Amigo" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para visitar um usuario correspondente: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

    executeQuery("select * from amizade where (cod_solicitado = '"+id_usr_vis+"' or cod_solicitador = '"+id_usr_vis+"') and condicao = '"+"1"+"'");
    res = mysql_store_result(conn);

    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            if(id_usr_vis == row[1]){
                id_usr_vis = row[0];
                executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
                res1 = mysql_store_result(conn);
                row1=mysql_fetch_row(res1);

                id_mural_vis = row1[8];

            }
            else{
                id_usr_vis = row[1];
                executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
                res1 = mysql_store_result(conn);
                row1=mysql_fetch_row(res1);

                id_mural_vis = row1[8];
            }
        }
    }

    return 1;
}


int verMembrosADM(){
    executeQuery("select * from associacao where cod_grp = '"+id_grp_vis+"' and (condicao = '"+"1"+"' or condicao = '"+"2"+"')");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "MEMBROS" << endl << endl;

        string id = row[1];

        executeQuery("select * from usuario where cod_usr = '"+id+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        cout << "--------------------------------------------------------" << endl;
        cout << ++i <<". "<< row1[2] << " " << row1[3] << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    if(!row&&i==0){
        cout << "Nenhum Membro" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para alterar o status do membro: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

   executeQuery("select * from associacao where cod_grp = '"+id_grp_vis+"' and (condicao = '"+"1"+"' or condicao = '"+"2"+"')");
   res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            string id = row[1];
            string condicao = row[2];
            int opt;
            system("cls");

            cout << "Selelcione uma das opções :" << endl;

            cout << "1. Visitar Perfil" << endl;
            if( id != id_usr){
                cout << "2. Remover do Grupo" << endl;
                if(condicao == "1"){
                    cout << "3. Tornar Administrador" << endl;
                }else{
                    cout << "3. Retirar Administrador" << endl;
                }
                cout << "4. Bloquear do Grupo" << endl;
            }

            cin >> opt;

            if(opt < 1 || opt > 4)
                opt = 1;
            switch(opt){
            case 1:
                if(id == id_usr)
                    return -1;
                id_usr_vis = id;
                executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
                res1 = mysql_store_result(conn);
                row1=mysql_fetch_row(res1);

                id_mural_vis = row1[8];

                return 1;
                break;
            case 2:
                if( id != id_usr)
                    executeQuery("delete from associacao where cod_grp = '"+id_grp_vis+"' and cod_usr = '"+id+"'");
                break;
            case 3:
                if( id != id_usr){
                    if(condicao == "1"){
                        executeQuery("update associacao set condicao = 2 where cod_grp = '"+id_grp_vis+"' and cod_usr = '"+id+"'");
                    }else{
                        executeQuery("update associacao set condicao = 1 where cod_grp = '"+id_grp_vis+"' and cod_usr = '"+id+"'");
                    }
                }
                break;
            case 4:
                string aux = id_usr_vis;
                id_usr_vis = id;

                executeQuery("update associacao set condicao = 3 where cod_grp = '"+id_grp_vis+"' and cod_usr = '"+id+"'");

                excluirRespostaBloq();
                excluirComentarioBloq();
                excluirPostagemBloq();

                id_usr_vis = aux;
            }
            return 0;
        }
    }
    return 0;
}

int verMembros(){
    executeQuery("select * from associacao where cod_grp = '"+id_grp_vis+"' and (condicao = '"+"1"+"' or condicao = '"+"2"+"')");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "MEMBROS" << endl << endl;

        string id = row[1];

        executeQuery("select * from usuario where cod_usr = '"+id+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        cout << "--------------------------------------------------------" << endl;
        cout << ++i <<". "<< row1[2] << " " << row1[3] << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    if(!row&&i==0){
        cout << "Nenhum Membro" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para visitar um usuario correspondente: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

   executeQuery("select * from associacao where cod_grp = '"+id_grp_vis+"' and (condicao = '"+"1"+"' or condicao = '"+"2"+"')");
   res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            id_usr_vis = row[1];

            executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
            res1 = mysql_store_result(conn);
            row1=mysql_fetch_row(res1);

            id_mural_vis = row[8];
        }
    }

    return 1;
}

int verAmigosEmComum(){
    executeQuery("select * from amizade where (cod_solicitado = '"+id_usr+"' or cod_solicitador = '"+id_usr+"') and condicao = '"+"1"+"'");
    res = mysql_store_result(conn);
    vector <string> amigos0;

    while(row=mysql_fetch_row(res)){
        string id;

        if(id_usr == row[1])
            id = row[0];
        else
            id = row[1];

        amigos0.push_back(id);
    }

    executeQuery("select * from amizade where (cod_solicitado = '"+id_usr_vis+"' or cod_solicitador = '"+id_usr_vis+"') and condicao = '"+"1"+"'");
    res = mysql_store_result(conn);
    vector <string> amigos1;

    while(row=mysql_fetch_row(res)){
        string id;

        if(id_usr_vis == row[1])
            id = row[0];
        else
            id = row[1];

        amigos1.push_back(id);
    }
    if(!row){
        int i;
        cout << "Nenhum amigo em comum" << endl;
        cin >> i;
        return 0;
    }



    vector <string> amigoscomum;

    for(int k = 0; k < amigos0.size(); k++){
        for(int j = 0; j < amigos0.size(); j++){
            if(amigos0[k] == amigos1[j])
                amigoscomum.push_back(amigos0[k]);
    }
    }

    int i;
    int z = 0;
    for(i = 0; i < amigoscomum.size(); i++){
        executeQuery("select * from usuario where cod_usr = '"+amigoscomum[i]+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        cout << "--------------------------------------------------------" << endl;
        cout << ++z <<". "<< row1[2] << " " << row1[3] << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para visitar um usuario correspondente: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

    id_usr_vis = amigoscomum[reg - 1];

    executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
    res1 = mysql_store_result(conn);
    row1=mysql_fetch_row(res1);

    id_mural_vis = row[8];

    return 1;
}

int bloqueadosGrupo(){
    executeQuery("select * from associacao where cod_grp = '"+id_grp_vis+"' and condicao = '"+"3"+"'");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "USUARIOS BLOQUEADOS" << endl << endl;

        string id = row[1];

        executeQuery("select * from usuario where cod_usr = '"+id+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        cout << "--------------------------------------------------------" << endl;
        cout << ++i <<". "<< row1[2] << " " << row1[3] << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    if(!row&&i==0){
        cout << "Nenhum Usuario Bloqueado" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para alterar o status do usuario correspondente: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

    executeQuery("select * from associacao where cod_grp = '"+id_grp_vis+"' and condicao = '"+"3"+"'");
    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            system("cls");

            string bloqueado = row[1];
            int opt;
            cout << "1. remover bloqueio"<< endl;
            cin >> opt;

            if(opt == 1)
                executeQuery("delete from associacao where cod_grp = '"+id_grp_vis+"' and cod_usr = '"+bloqueado+"'");
            else
                return 1;
        }
    }

    return 1;
}

int verBloqueados(){
    string muralvisitado = id_mural_vis;
    id_mural_vis = id_mural;

    string idusuario = id_usr;
    string idvisitado = id_usr_vis;

    executeQuery("select * from bloqueio where cod_bloqueador = '"+id_usr+"'");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "USUARIOS BLOQUEADOS" << endl << endl;

        string id = row[1];
        executeQuery("select * from usuario where cod_usr = '"+id+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        cout << "--------------------------------------------------------" << endl;
        cout << ++i <<". "<< row1[2] << " " << row1[3] << endl;
        cout << "--------------------------------------------------------" << endl;
    }

    if(!row&&i==0){
        cout << "Nenhum Usuario Bloqueado" << endl;
        return 0;
    }

    int reg = 0;
    cout << "Digite 0 para voltar ou algum numero para aceitar ou nao a solicitacao correspondente: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

    executeQuery("select * from bloqueio where cod_bloqueador = '"+id_usr+"'");
    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            system("cls");

            string bloqueador = row[0], bloqueado = row[1];
            int opt;
            cout << "1. remover bloqueio         0. voltar"<< endl;
            cin >> opt;

            if(opt == 1){
                executeQuery("delete from bloqueio where cod_bloqueador = '"+bloqueador+"' and cod_bloqueado = '"+bloqueado+"'");

                executeQuery("select * from associacao where cod_usr = '"+id_usr+"' and condicao = '"+"2"+"'");
                res = mysql_store_result(conn);

                vector<string> idGrupo;

                while(row = mysql_fetch_row(res))
                    idGrupo.push_back(row[0]);

                for(int i = 0; i < idGrupo.size(); i++){
                    executeQuery("delete from associacao where cod_grp = '"+idGrupo[i]+"' and cod_usr = '"+id_usr_vis+"'");
                }

                id_mural_vis = muralvisitado;
                id_usr = idvisitado;
                id_usr_vis = idusuario;

                executeQuery("select * from associacao where cod_usr = '"+id_usr+"' and condicao = '"+"2"+"'");
                res = mysql_store_result(conn);

                vector<string> idGrupo2;

                while(row = mysql_fetch_row(res))
                    idGrupo2.push_back(row[0]);

                for(int i = 0; i < idGrupo2.size(); i++){
                    executeQuery("delete from associacao where cod_grp = '"+idGrupo2[i]+"' and cod_usr = '"+id_usr_vis+"'");
                }

                id_usr = idusuario;
                id_usr_vis = idvisitado;

            }
            else
                return 0;
        }
    }

    return 1;
}

////////////////////////////
//////////////////////////// VER INFORMAÇÕES GRUPO OU PESSOA
void nomeUsuario(){
    executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    cout << "--------------------------------------------------------" << endl;
    cout << row[4] << endl;
    cout << row[2] << " " << row[3] << endl;
    cout << "--------------------------------------------------------" << endl;
}

void nomeGrupo(){
    executeQuery("select * from grupo where cod_grp = '"+id_grp_vis+"'");
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    cout << "--------------------------------------------------------" << endl;
    cout << row[3] << endl;
    cout << row[1] << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << row[2] << endl;
    cout << "--------------------------------------------------------" << endl;

}
/////////////////////////
//////////////////////// VERIFICADOR DE VISIBILIDADE
int checarVisibilidade(){
    executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    return atoi(row[6]);
}

int checarVisibilidadeGrp(){
    executeQuery("select * from grupo where cod_grp = '"+id_grp_vis+"'");
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    return atoi(row[4]);
}
////////////////////////
/////////////////////// DESFAZER ASSOCIACAO OU AMIZADE
void desfazerAmizade(){
    executeQuery("delete from amizade where (cod_solicitador = '"+id_usr+"' and cod_solicitado = '"+id_usr_vis+"') or (cod_solicitador = '"+id_usr_vis+"' and cod_solicitado = '"+id_usr+"')");
}

void desfazerAssociacao(){
    executeQuery("delete from associacao where cod_grp = '"+id_grp_vis+"' and cod_usr = '"+id_usr+"'");
}
/////////////////////
//////////////////// EDITAR INFORMAÇÕES DE USUARIOS OU GRUPOS

void editarPerfil(){
    system("cls");

    cout << "Selecione oque deseja editar" << endl;

    cout << "1. Nome" << endl;
    cout << "2. Sobrenome" << endl;
    cout << "3. Cidade" << endl;
    cout << "4. Visibilidade" << endl;
    cout << "5. Senha" << endl;
    cout << "6. Foto" << endl;

    int opt;
    cin >> opt;

    switch(opt){
        case 1:{
            system("cls");
            cout << "Digite o seu novo nome: "<< endl;

            string nome;
            cin >> nome;

            executeQuery("update usuario set nome = '"+nome+"' where cod_usr = '"+id_usr+"'");
            break;
        }
        case 2:{
            system("cls");
            cout << "Digite o seu novo sobrenome: "<< endl;

            string sobrenome;
            cin >> sobrenome;

            executeQuery("update usuario set sobrenome = '"+sobrenome+"' where cod_usr = '"+id_usr+"'");
            break;
        }
        case 3:{
            system("cls");
            cout << "Digite o sua nova cidade: "<< endl;

            string cidade;
            cin >> cidade;

            executeQuery("update usuario set cidade = '"+cidade+"' where cod_usr = '"+id_usr+"'");
            break;
        }
        case 4:{
            system("cls");
            cout << "Digite o sua nova opçao de visibilidade: "<< endl;

            cout << "0. Amigo"<< endl;
            cout << "1. Amigo de Amigo"<< endl;
            cout << "2. Publico"<< endl;

            string visibilidade;
            cin >> visibilidade;

            if(visibilidade != "0" && visibilidade != "1" && visibilidade != "2")
                visibilidade = "0";

            executeQuery("update usuario set visibilidade = '"+visibilidade+"' where cod_usr = '"+id_usr+"'");
            break;
        }
        case 5:{
            system("cls");
            cout << "Digite o sua nova senha: "<< endl;

            string senha;
            cin >> senha;

            executeQuery("update usuario set senha = '"+senha+"' where cod_usr = '"+id_usr+"'");
            break;
        }
        case 6:{
            system("cls");
            cout << "Digite o sua nova foto: "<< endl;

            string foto;
            cin >> foto;

            executeQuery("update usuario set foto = '"+foto+"' where cod_usr = '"+id_usr+"'");
        }
    }
}

void editarGrupo(){
    system("cls");

    cout << "Selecione oque deseja editar" << endl;

    cout << "1. Nome" << endl;
    cout << "2. Descricao" << endl;
    cout << "3. Visibilidade" << endl;
    cout << "4. Foto" << endl;

    int opt;
    cin >> opt;

    switch(opt){
        case 1:{
            system("cls");
            cout << "Digite o novo nome do grupo: "<< endl;

            string nome;
            cin >> nome;

            executeQuery("update grupo set nome = '"+nome+"' where cod_grp = '"+id_grp_vis+"'");
            break;
        }
        case 2:{
            system("cls");
            cout << "Digite a nova descricao: "<< endl;

            string descricao;
            cin >> descricao;

            executeQuery("update grupo set descricao = '"+descricao+"' where cod_grp = '"+id_grp_vis+"'");
            break;
        }
        case 3:{
            system("cls");
            cout << "Digite a nova opçao de visibilidade: "<< endl;

            cout << "0. Privado"<< endl;
            cout << "1. Publico"<< endl;

            string visibilidade;
            cin >> visibilidade;

            if(visibilidade != "0" && visibilidade != "1")
                visibilidade = "0";

            executeQuery("update grupo set visibilidade = '"+visibilidade+"' where cod_grp = '"+id_grp_vis+"'");
            break;
        }
        case 4:{
            system("cls");
            cout << "Digite o sua nova foto: "<< endl;

            string foto;
            cin >> foto;

            executeQuery("update grupo set foto = '"+foto+"' where cod_grp = '"+id_grp_vis+"'");
        }
    }
}
//////////////////////////
////////////////////////// CHECAGEM SE EH DONO DO MURAL, DONO DO CONTEUDO OU ADMINISTRADOR DO GRUPO
int donoMural(int tipo){
    if(tipo == 0){
        executeQuery("select * from postagem where cod_post = '"+id_pub_vis+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string mural = row[3];

        executeQuery("select * from usuario where cod_usr = '"+id_usr+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string mural1 = row[8];

        if(mural == mural1){
                cout << "deu ruim0" << endl;
            return 1;
        }
        return 0;
    }
    if(tipo == 1){
        executeQuery("select * from comentario where cod_coment = '"+id_cmnt_vis+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string mural = row[2];

        executeQuery("select * from usuario where cod_usr = '"+id_usr+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string mural1 = row[8];

        if(mural == mural1)
            return 1;
        return 0;
    }
    if(tipo == 2){
        executeQuery("select * from resposta where cod_resp = '"+id_resp_vis+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string mural = row[2];

        executeQuery("select * from usuario where cod_usr = '"+id_usr+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string mural1 = row[8];

        if(mural == mural1)
            return 1;
        return 0;
    }
}

int donoConteudo(int tipo){
    if(tipo == 0){
        executeQuery("select * from postagem where cod_post = '"+id_pub_vis+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string id = row[4];

        if(id == id_usr){
            return 1;
        }
        return 0;
    }
    if(tipo == 1){
        executeQuery("select * from comentario where cod_coment = '"+id_cmnt_vis+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string id = row[3];

        if(id == id_usr)
            return 1;
        return 0;
    }
    if(tipo == 2){
        executeQuery("select * from resposta where cod_resp = '"+id_resp_vis+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        string id = row[3];

        if(id == id_usr)
            return 1;
        return 0;
    }
}

int donoGrupo(int tipo){
    if(tipo == 0){
        string muralpost = id_mural_vis;

        executeQuery("select * from grupo where cod_wall = '"+muralpost+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        if(!row)
            return 0;

        string grupo = row[0];
        executeQuery("select * from associacao where cod_grp = '"+grupo+"' and cod_usr = '"+id_usr+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if(!row)
            return 0;

        string condicao = row[2];

        if(condicao == "2")
            return 1;

        return 0;
    }
    if(tipo == 1){
        string muralpost = id_mural_vis;

        executeQuery("select * from grupo where cod_wall = '"+muralpost+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if(!row)
            return 0;

        string grupo = row[0];

        executeQuery("select * from associacao where cod_grp = '"+grupo+"' cod_usr = '"+id_usr+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if(!row)
            return 0;

        string condicao = row[2];

        if(condicao == "2")
            return 1;
        return 0;
    }
    if(tipo == 2){
        string muralpost = id_mural_vis;

        executeQuery("select * from grupo where cod_wall = '"+muralpost+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if(!row)
            return 0;

        string grupo = row[0];

        executeQuery("select * from associacao where cod_grp = '"+grupo+"' cod_usr = '"+id_usr+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        if(!row)
            return 0;

        string condicao = row[2];

        if(condicao == "2")
            return 1;
        return 0;
    }
}
/////////////////////////////
///////////////////////////// APAGAR PUBLICAÇÕES, COMENTARIOS, RESPOSTAS
void excluirResposta(){
    executeQuery("delete from resposta where cod_resp = '"+id_resp_vis+"'");
}

void excluirComentario(){
    executeQuery("select * from resposta where cod_coment = '"+id_cmnt_vis+"'");
    res = mysql_store_result(conn);

    vector<string> id;

    while(row = mysql_fetch_row(res))
        id.push_back(row[0]);

    for(int i = 0; i < id.size(); i++)
        executeQuery("delete from resposta where cod_resp = '"+id[i]+"'");

    executeQuery("delete from comentario where cod_resp = '"+id_cmnt_vis+"'");
}

void excluirPostagem(){
    executeQuery("select * from resposta where cod_post = '"+id_pub_vis+"'");
    res = mysql_store_result(conn);

    vector<string> id;

    while(row = mysql_fetch_row(res))
        id.push_back(row[0]);

    for(int i = 0; i < id.size(); i++)
        executeQuery("delete from resposta where cod_resp = '"+id[i]+"'");


    executeQuery("select * from comentario where cod_post = '"+id_pub_vis+"'");
    res = mysql_store_result(conn);

    vector<string> id1;

    while(row = mysql_fetch_row(res))
        id1.push_back(row[0]);

    for(int i = 0; i < id1.size(); i++)
        executeQuery("delete from comentario where cod_coment = '"+id1[i]+"'");

    executeQuery("delete from postagem where cod_post = '"+id_pub_vis+"'");
}

void bloquearUsr(){
    string muralvisitado = id_mural_vis;
    id_mural_vis = id_mural;

    string idusuario = id_usr;
    string idvisitado = id_usr_vis;

    excluirRespostaBloq();
    excluirComentarioBloq();
    excluirPostagemBloq();

    executeQuery("delete from amizade where (cod_solicitador = '"+id_usr_vis+"' and cod_solicitado = '"+id_usr+"') or (cod_solicitador = '"+id_usr+"' and cod_solicitado = '"+id_usr_vis+"')");

    executeQuery("select * from associacao where cod_usr = '"+id_usr+"' and condicao = '"+"2"+"'");
    res = mysql_store_result(conn);

    vector<string> idGrupo;

    while(row = mysql_fetch_row(res))
        idGrupo.push_back(row[0]);

    for(int i = 0; i < idGrupo.size(); i++){
        executeQuery("select * from grupo where cod_grp = '"+idGrupo[i]+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        id_mural_vis = row[5];

        excluirRespostaBloq();
        excluirComentarioBloq();
        excluirPostagemBloq();

        executeQuery("delete from associacao where cod_grp = '"+idGrupo[i]+"' and cod_usr = '"+id_usr_vis+"'");
        executeQuery("insert into associacao(cod_grp, cod_usr, condicao) values('"+idGrupo[i]+"', '"+id_usr_vis+"', '"+"4"+"')");
    }

    id_mural_vis = muralvisitado;
    id_usr = idvisitado;
    id_usr_vis = idusuario;

    excluirRespostaBloq();
    excluirComentarioBloq();
    excluirPostagemBloq();

    executeQuery("delete from amizade where (cod_solicitador = '"+id_usr_vis+"' and cod_solicitado = '"+id_usr+"') or (cod_solicitador = '"+id_usr+"' and cod_solicitado = '"+id_usr_vis+"')");

    executeQuery("select * from associacao where cod_usr = '"+id_usr+"' and condicao = '"+"2"+"'");
    res = mysql_store_result(conn);

    vector<string> idGrupo2;

    while(row = mysql_fetch_row(res))
        idGrupo2.push_back(row[0]);

    for(int i = 0; i < idGrupo2.size(); i++){
        executeQuery("select * from grupo where cod_grp = '"+idGrupo2[i]+"'");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        id_mural_vis = row[5];

        excluirRespostaBloq();
        excluirComentarioBloq();
        excluirPostagemBloq();

        executeQuery("delete from associacao where cod_grp = '"+idGrupo2[i]+"' and cod_usr = '"+id_usr_vis+"'");
        executeQuery("insert into associacao(cod_grp, cod_usr, condicao) values('"+idGrupo2[i]+"', '"+id_usr_vis+"', '"+"4"+"')");
    }

    id_usr = idusuario;
    id_usr_vis = idvisitado;

    executeQuery("insert into bloqueio(cod_bloqueador, cod_bloqueado) values('"+id_usr+"', '"+id_usr_vis+"')");
}



