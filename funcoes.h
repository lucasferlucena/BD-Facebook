#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <string>
#include <stdlib.h>

using namespace std;

MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;
MYSQL_ROW row1;
MYSQL_RES* res1;
int qstate;

string id_usr = "";

string id_usr_vis = "";
string id_grp_vis = "";

string id_pub_vis = "";
string id_cmnt_vis = "";

void executeQuery(string query){
     const char* q = query.c_str();
     qstate = mysql_query(conn,q);
}

int cadastrarUsuario(){
    system("cls");
    string nome, sobrenome, email, senha, cidade, foto, cod_wall;

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

        executeQuery("insert into mural() values()");

        qstate = mysql_query(conn ,"SELECT LAST_INSERT_ID()");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        cod_wall = row[0];
        executeQuery("insert into usuario(nome,sobrenome,email,senha,cidade,foto,cod_wall) values('"+nome+"','"+sobrenome+"','"+email+"','"+senha+"','"+cidade+"','"+foto+"','"+cod_wall+"')");

        qstate = mysql_query(conn ,"SELECT LAST_INSERT_ID()");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        id_usr = row[0];
        id_usr_vis = id_usr;

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
    string nome, descricao, foto, cod_wall;

    if(conn){
        cout << "Digite o nome do grupo: "<< endl;
        cin >> nome;
        cin.ignore(80, '\n');
        cout << "Digite a descricao: "<< endl;
        getline(cin, descricao);
        cout << "Registre sua foto: "<< endl;
        cin >> foto;

        executeQuery("insert into mural() values()");

        qstate = mysql_query(conn ,"SELECT LAST_INSERT_ID()");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);

        cod_wall = row[0];

        executeQuery("insert into grupo(nome,descricao,foto,visibilidade,cod_wall) values('"+nome+"','"+descricao+"','"+foto+"','"+"0"+"','"+cod_wall+"')");

        if(!qstate)
            cout<<"Grupo "<< nome <<" criado com sucesso"<<endl;
        else
            cout<<"query problem: "<<mysql_error(conn)<<endl;


        qstate = mysql_query(conn ,"SELECT LAST_INSERT_ID()");
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        string id_grupo = row[0];

        executeQuery("insert into associacao(cod_grp, cod_usr, condicao) values('"+id_grupo+"','"+id_usr+"','"+"0"+"')");
    }
    else
    {
        cout<<"connection problem: "<<mysql_error(conn)<<endl;
    }
}

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
        return 1;
    }
    else
        cout <<"Acesso negado!"<<endl;
        return 0;
    }
}

int buscarPessoas(){
    system("cls");
    string nome;

    cout << "Digite o primeiro nome da pessoa que voce gostaria de pesquisar: " << endl;
    cin >> nome;

    executeQuery("select * from usuario where nome = '"+nome+"'");

    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "RESULTADOS:" << endl;

        cout << ++i << ": " << row[2] << " " << row[3] << endl;
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

    executeQuery("select * from usuario where nome = '"+nome+"'");

    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            id_usr_vis = row[0];
        }
    }

    return 1;
}

int buscarGrupos(){
    system("cls");
    string nome;

    cout << "Digite o nome do grupo que voce gostaria de pesquisar: " << endl;
    cin >> nome;

    executeQuery("select * from grupo where nome = '"+nome+"'");

    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "RESULTADOS:" << endl;

        cout << ++i << ": " << row[1] << endl;
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

    executeQuery("select * from grupo where nome = '"+nome+"'");

    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            id_grp_vis = row[0];
            cout << row[1];
        }
    }
    return 1;
}

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

    executeQuery("insert into postagem(texto, foto, cod_wall, cod_usr) values('"+texto+"','"+foto+"','"+id_wall+"','"+id_usr+"')");

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

    executeQuery("insert into comentario(texto, cod_wall, cod_usr, cod_post) values('"+texto+"','"+id_wall+"','"+id_usr+"','"+id_pub_vis+"')");

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

    executeQuery("insert into resposta(texto, cod_wall, cod_usr, cod_post, cod_coment) values('"+texto+"','"+id_wall+"','"+id_usr+"','"+id_pub_vis+"','"+id_cmnt_vis+"')");

    return 1;
}

int verPublicacoes(){
    system("cls");
    string texto = "", foto = "", nome_usr_pub = "", sobrenome_usr_pub = "", id_wall, nome_usr_mural, sobrenome_usr_mural;

    executeQuery("select * from usuario where cod_usr = '"+id_usr_vis+"'");
    res = mysql_store_result(conn);
    row=mysql_fetch_row(res);
    id_wall = row[8];
    nome_usr_mural = row[2];
    sobrenome_usr_mural = row[3];

    executeQuery("select * from postagem where cod_wall = '"+id_wall+"'");
    res = mysql_store_result(conn);
    int i = 0;

    while(row=mysql_fetch_row(res)){
        if(!i)
            cout<< "PUBLICACOES NO MURAL DE "<< nome_usr_mural << " "<< sobrenome_usr_mural <<":" << endl << endl;
        string id = row[4];
        executeQuery("select * from usuario where cod_usr = '"+id+"'");
        res1 = mysql_store_result(conn);
        row1 = mysql_fetch_row(res1);

        nome_usr_pub = row1[2];
        sobrenome_usr_pub = row1[3];

        cout << "--------------------------------------------------------" << endl;
        cout << "#Publicacao" << ++i << endl << endl;
        cout << nome_usr_pub << " " << sobrenome_usr_pub << endl;
        if(row[2] != "")
            cout << row[2] << endl;
        if(row[1] != "")
            cout << row[1] << endl;
        cout << "--------------------------------------------------------" << endl;
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

    executeQuery("select * from postagem where cod_wall = '"+id_wall+"'");
    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            id_pub_vis = row[0];
            cout << row[1];
        }
    }

    return 1;
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

        cout << "--------------------------------------------------------" << endl;
        cout << "#Comentario" << ++i << endl << endl;
        cout << nome_usr_comment << " " << sobrenome_usr_comment << endl;
        cout << row[1] << endl;
        cout << "--------------------------------------------------------" << endl;
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
        if(++i == reg){
            id_cmnt_vis = row[0];
            cout << row[1];
        }
    }

    return 1;
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

        cout << "--------------------------------------------------------" << endl;
        cout << "#Resposta" << ++i << endl << endl;
        cout << nome_usr_reply << " " << sobrenome_usr_reply << endl;
        cout << row[1] << endl;
        cout << "--------------------------------------------------------" << endl;
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
}

int amizadeVerificador(){
    executeQuery("select * from amizade where (cod_solicitador ='"+id_usr+"' and cod_solicitado = '"+id_usr_vis+"') or (cod_solicitador ='"+id_usr_vis+"' and cod_solicitado = '"+id_usr+"')");
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    if(id_usr_vis == id_usr)
        return 5;
    if(!row)
        return -1;
    return atoi(row[2]);
}

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
    cout << "Digite 0 para voltar ou algum numero para aceitar a solicitacao correspondente: " << endl;
    cin >> reg;

    if(!reg || reg > i)
        return 0;

    executeQuery("select * from amizade where cod_solicitado = '"+id_usr+"' and condicao = '"+"0"+"'");
    res = mysql_store_result(conn);
    i = 0;

    while(row=mysql_fetch_row(res)){
        if(++i == reg){
            string solicitador = row[0], solicitado = row[1];
            executeQuery("update amizade set condicao = 1 where cod_solicitador = '"+solicitador+"' and cod_solicitado = '"+solicitado+"'");
        }
    }

    return 1;

}

void solicitarAmizade(){
    executeQuery("insert into amizade(cod_solicitador,cod_solicitado,condicao) values('"+id_usr+"','"+id_usr_vis+"','"+"0"+"')");
}



