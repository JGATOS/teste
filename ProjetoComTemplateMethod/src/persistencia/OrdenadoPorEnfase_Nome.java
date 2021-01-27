/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package persistencia;

import classededados.Aluno;

/**
 *
 * @author jgatosantana
 */
public class OrdenadoPorEnfase_Nome extends AlunoPersistenciaTemplateMethod{
    public OrdenadoPorEnfase_Nome (String nome) {
        super(nome);
    }
    
    @Override
    public boolean ePrimeiro(Aluno aluno1, Aluno aluno2) {
       if(aluno1.getEnfase().compareToIgnoreCase(aluno2.getEnfase()) < 0) return true;
       else if (aluno1.getEnfase().compareToIgnoreCase(aluno2.getEnfase()) > 0) return false;
       else if (aluno1.getEnfase().compareToIgnoreCase(aluno2.getEnfase()) == 0){
          
           if(aluno1.getNome().compareToIgnoreCase(aluno2.getNome()) <= 0)return true;
           else if(aluno1.getNome().compareToIgnoreCase(aluno2.getNome()) > 0) return false;
       } 
       return false;
    } 
}
