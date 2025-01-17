#======================================================================
# Trabalho de Compiladores
# tradutor : json para maspy
#======================================================================

from maspy import *

class Agente1(Agent):
	def __init__(self, agt_name):
		super().__init__(agt_name)
		self.add(Belief("estaChovendo"))
		self.add(Belief("naotenhoGuardaChuva"))
		self.add(Belief("tenhoProva"))
		self.add(Goal("comprarGuardaChuva"))

	@pl(gain, Goal("comprarGuardaChuva"), Belief("estaChovendo"))
	def comprarGuardaChuva_(self, src):
		self.print("sairparacomprarguardachuva")
		self.print("procurarloja")
		self.print("comprarguardachuva")
		self.add(Goal("estudar"))

	@pl(gain, Goal("estudar"), Belief("tenhoProva"))
	def estudar_(self, src):
		self.print("convidaralice")
		self.print("estudarnabiblioteca")

class Agente2(Agent):
	def __init__(self, agt_name):
		super().__init__(agt_name)
		self.add(Belief("estaChovendo"))
		self.add(Belief("tenhoGuardaChuva"))
		self.add(Belief("tenhoProva"))

	@pl(gain, Belief("tenhoGuardaChuva"), Belief("estaChovendo"))
	def tenhoGuardaChuva_(self, src):
		self.print("possoestudar")
		self.print("recebiconvitedebob")
		self.add(Goal("estudarNaBiblioteca"))

	@pl(gain, Goal("estudarNaBiblioteca"), Belief("tenhoProva"))
	def estudarNaBiblioteca_(self, src):
		self.print("escolhercaminho")
		self.print("usarguardachuva")
		self.print("irparabiblioteca")
		self.print("estudar")

Agente1("bob")
Agente2("alice")
Admin().start_system()