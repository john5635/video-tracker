LATEX = latex
BIBTEX = bibtex
L2H = latex2html
PDFLATEX = ps2pdf
DVIPS = dvips

RERUN = "(There were undefined references|Rerun to get (cross-references|the bars) right)"
RERUNBIB = "No file.*\.bbl|Citation.*undefined"

SOURCES = tesi.tex

GOALS = tesi.ps tesi.pdf
DVIFILES = tesi.dvi

COPY = if test -r $*.toc; then cp $*.toc $*.toc.bak; fi 
RM = /bin/rm -f

main:        	$(DVIFILES)

all:            $(GOALS)

%.dvi:          *.tex
				$(COPY);
				$(LATEX) $(SOURCES)
				$(BIBTEX) appunti
				egrep -c $(RERUNBIB) $*.log && ($(BIBTEX) $*;$(COPY);$(LATEX) $(SOURCES)) ; true
				egrep $(RERUN) $*.log && ($(COPY);$(LATEX) $(SOURCES)) ; true
				egrep $(RERUN) $*.log && ($(COPY);$(LATEX) $(SOURCES)) ; true
				$(LATEX) $(SOURCES); 
				if cmp -s $*.toc $*.toc.bak; then echo ;else $(LATEX) $(SOURCES) ; fi
				$(RM) $*.toc.bak
# Display relevant warnings
				egrep -i "(Reference|Citation).*undefined" $*.log ; true

%.ps:           %.dvi
				dvips $< -o $@

%.pdf:          %.ps
				$(PDFLATEX) $<


fast:			*.tex
				$(LATEX) $(SOURCES)
				dvips tesi.dvi -o tesi.ps

clean:

				rm -f *.aux *.log *.blg *.brf *.cb *.ind *.idx *.ilg  \
				*.inx *.toc *.out *.bbl *.lof *.lot *.backup *.ps *.dvi *~ #*.pdf

info:				
				@echo "Tesi di laurea - Autore: Marco Meoni - Titolo: Progettazione e sviluppo di un Network Management System per reti wireless in configurazione MeshAP - A.A. 2004/2005"
