# .bashrc

alias vi="vim"
alias rm="rm -i"
alias mv="mv -i"
alias cdbcm="cd /other/sdk-xgs-robo-5.6.6"

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific aliases and functions
export PATH=/opt2/montavista/pro/devkit/ppc/82xx/bin:$PATH
export PS1="\[\e[1;32m\][\[\e[0;1;5;35m\]\u\[\e[0;1;31m\]@\h \[\e[36m\]\W\[\e[32m\]]\$ \[\e[0m\]"
source ~/.git-completion.bash
