/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package classededados;

/**
 *
 * @author eugeniojulio
 */
public class Aluno {
    //Atributos
    private String curso;
    private String nome;
    private String enfase;
    private String situacao;
    //Metodos
    public Aluno(String matricula, String nome, String enfase, String situacao){
        this.curso = curso;
        this.nome = nome;
        this.enfase = enfase;
        this.situacao = situacao;
    }
    public Aluno(String str){
        String[] vetor = str.split(";");
        this.nome = vetor[0];
        this.curso = vetor[1];
        this.situacao = vetor[2];
        this.enfase = vetor[3];
    }

    /**
     * @return the matricula
     */


    /**
     * @return the nome
     */
    public String getNome() {
        return nome;
    }

    /**
     * @param nome the nome to set
     */
    public void setNome(String nome) {
        this.nome = nome;
    }

    /**
     * @return the curso
     */
    public String getCurso() {
        return curso;
    }

    /**
     * @param curso the curso to set
     */
    public void setCurso(String curso) {
        this.curso = curso;
    }

    /**
     * @return the enfase
     */
    public String getEnfase() {
        return enfase;
    }

    /**
     * @param enfase the enfase to set
     */
    public void setEnfase(String enfase) {
        this.enfase = enfase;
    }

    /**
     * @return the situacao
     */
    public String getSituacao() {
        return situacao;
    }

    /**
     * @param situacao the situacao to set
     */
    public void setSituacao(String situacao) {
        this.situacao = situacao;
    }
    
}
