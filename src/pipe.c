///* ************************************************************************** */
///*                                                                            */
///*                                                        :::      ::::::::   */
///*   pipe.c                                             :+:      :+:    :+:   */
///*                                                    +:+ +:+         +:+     */
///*   By: apechkov <apechkov@student.42.fr>          +#+  +:+       +#+        */
///*                                                +#+#+#+#+#+   +#+           */
///*   Created: 2024/11/15 16:28:58 by apechkov          #+#    #+#             */
///*   Updated: 2025/01/31 17:24:09 by apechkov         ###   ########.fr       */
///*                                                                            */
///* ************************************************************************** */

//#include "../minishell.h"

////exit | exit | exit (shouldn't exit and shouldn't print anything) (exit status 0)
////export GHOST=123 | env | grep GHOST (exit status 1)


//need to fix
// [0;31m———————————— pipes
// [0;33mTest   1: ❌[38;5;244m env | sort | grep -v SHLVL | grep -v ^_ 
// [0mmini output = ()
// bash output = (CHROME_DESKTOP=code.desktop COLORTERM=truecolor DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus,guid=f2d9ab633147a4d63f230cc267b1ddb3 DBUS_STARTER_ADDRESS=unix:path=/run/user/1000/bus,guid=f2d9ab633147a4d63f230cc267b1ddb3 DBUS_STARTER_BUS_TYPE=session DEBUGINFOD_URLS=https://debuginfod.ubuntu.com DESKTOP_SESSION=ubuntu DISPLAY=:1 FONTCONFIG_FILE=/etc/fonts/fonts.conf FONTCONFIG_PATH=/etc/fonts GDK_BACKEND_VSCODE_SNAP_ORIG= GDK_BACKEND=x11 GDMSESSION=ubuntu GIO_MODULE_DIR=/home/anastasiia/snap/code/common/.cache/gio-modules GIO_MODULE_DIR_VSCODE_SNAP_ORIG= GIT_ASKPASS=/snap/code/179/usr/share/code/resources/app/extensions/git/dist/askpass.sh GNOME_DESKTOP_SESSION_ID=this-is-deprecated GNOME_SHELL_SESSION_MODE=ubuntu GNOME_TERMINAL_SCREEN=/org/gnome/Terminal/screen/bcf7fbb0_4fdc_4f78_a05e_52d03768d5be GNOME_TERMINAL_SERVICE=:1.130 GPG_AGENT_INFO=/run/user/1000/gnupg/S.gpg-agent:0:1 GSETTINGS_SCHEMA_DIR=/home/anastasiia/snap/code/179/.local/share/glib-2.0/schemas GSETTINGS_SCHEMA_DIR_VSCODE_SNAP_ORIG= GSM_SKIP_SSH_AGENT_WORKAROUND=true GTK_EXE_PREFIX=/snap/code/179/usr GTK_EXE_PREFIX_VSCODE_SNAP_ORIG= GTK_IM_MODULE_FILE=/home/anastasiia/snap/code/common/.cache/immodules/immodules.cache GTK_IM_MODULE_FILE_VSCODE_SNAP_ORIG= GTK_MODULES=gail:atk-bridge GTK_PATH=/snap/code/179/usr/lib/x86_64-linux-gnu/gtk-3.0 GTK_PATH_VSCODE_SNAP_ORIG= HOME=/home/anastasiia LANG=en_US.UTF-8 LESS=-R LOCPATH=/snap/code/179/usr/lib/locale LOCPATH_VSCODE_SNAP_ORIG= LOGNAME=anastasiia LSCOLORS=Gxfxcxdxbxegedabagacad LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=00:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.avif=01;35:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:*~=00;90:*#=00;90:*.bak=00;90:*.crdownload=00;90:*.dpkg-dist=00;90:*.dpkg-new=00;90:*.dpkg-old=00;90:*.dpkg-tmp=00;90:*.old=00;90:*.orig=00;90:*.part=00;90:*.rej=00;90:*.rpmnew=00;90:*.rpmorig=00;90:*.rpmsave=00;90:*.swp=00;90:*.tmp=00;90:*.ucf-dist=00;90:*.ucf-new=00;90:*.ucf-old=00;90: MEMORY_PRESSURE_WATCH=/sys/fs/cgroup/user.slice/user-1000.slice/user@1000.service/app.slice/app-gnome\x2dsession\x2dmanager.slice/gnome-session-manager@ubuntu.service/memory.pressure MEMORY_PRESSURE_WRITE=c29tZSAyMDAwMDAgMjAwMDAwMAA= NVM_BIN=/home/anastasiia/.nvm/versions/node/v20.18.0/bin NVM_CD_FLAGS=-q NVM_DIR=/home/anastasiia/.nvm NVM_INC=/home/anastasiia/.nvm/versions/node/v20.18.0/include/node OLDPWD=/home/anastasiia/projects/minishell ORIGINAL_XDG_CURRENT_DESKTOP=ubuntu:GNOME P9K_SSH=0 P9K_TTY=old PAGER=less PATH=/home/anastasiia/.local/funcheck/host:/home/anastasiia/.nvm/versions/node/v20.18.0/bin:/home/anastasiia/.local/funcheck/host:/home/anastasiia/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin:/home/anastasiia/.config/Code/User/globalStorage/github.copilot-chat/debugCommand PWD=/home/anastasiia/projects/minishell/minishell_tester QT_ACCESSIBILITY=1 QT_IM_MODULE=ibus SESSION_MANAGER=local/anastasiia-Lenovo-Flex-2-Pro-15:@/tmp/.ICE-unix/2676,unix/anastasiia-Lenovo-Flex-2-Pro-15:/tmp/.ICE-unix/2676 SHELL=/usr/bin/zsh SSH_AUTH_SOCK=/run/user/1000/keyring/ssh SYSTEMD_EXEC_PID=2676 TERM_PROGRAM_VERSION=1.96.2 TERM_PROGRAM=vscode TERM=xterm-256color USER=anastasiia USERNAME=anastasiia USER_ZDOTDIR=/home/anastasiia VSCODE_GIT_ASKPASS_EXTRA_ARGS= VSCODE_GIT_ASKPASS_MAIN=/snap/code/179/usr/share/code/resources/app/extensions/git/dist/askpass-main.js VSCODE_GIT_ASKPASS_NODE=/snap/code/179/usr/share/code/code VSCODE_GIT_IPC_HANDLE=/run/user/1000/vscode-git-8f6a2af7c0.sock VSCODE_INJECTION=1 VSCODE_NONCE=d52550eb-78e4-4e80-91a8-1f22025078b6 VTE_VERSION=7600 WINDOWPATH=2 XAUTHORITY=/run/user/1000/gdm/Xauthority XDG_CONFIG_DIRS=/etc/xdg/xdg-ubuntu:/etc/xdg XDG_CONFIG_DIRS_VSCODE_SNAP_ORIG=/etc/xdg/xdg-ubuntu:/etc/xdg XDG_CURRENT_DESKTOP=Unity XDG_DATA_DIRS=/home/anastasiia/snap/code/179/.local/share:/home/anastasiia/snap/code/179:/snap/code/179/usr/share:/usr/share/ubuntu:/usr/share/gnome:/usr/local/share/:/usr/share/:/var/lib/snapd/desktop XDG_DATA_DIRS_VSCODE_SNAP_ORIG=/usr/share/ubuntu:/usr/share/gnome:/usr/local/share/:/usr/share/:/var/lib/snapd/desktop XDG_DATA_HOME=/home/anastasiia/snap/code/179/.local/share XDG_DATA_HOME_VSCODE_SNAP_ORIG= XDG_MENU_PREFIX=gnome- XDG_RUNTIME_DIR=/run/user/1000 XDG_SESSION_CLASS=user XDG_SESSION_DESKTOP=ubuntu XDG_SESSION_TYPE=x11 XMODIFIERS=@im=ibus ZDOTDIR=/home/anastasiia ZSH=/home/anastasiia/.oh-my-zsh)
// mini exit code = 127
// bash exit code = 0
// mini error = ( No such file or directory)
// bash error = ()
// [0;33mTest   2: ❌[38;5;244m cat ./test_files/infile_big | grep oi 
// [0mmini output = ()
// bash output = (was going to happen next. First, she tried to look down and make out disappointment it was empty: she did not like to drop the jar for fear not going to do _that_ in a hurry. “No, I’ll look first,” she said, “and see whether it’s marked ‘_poison_’ or not”; for she had read bottle marked “poison,” it is almost certain to disagree with you, However, this bottle was _not_ marked “poison,” so Alice ventured to brightened up at the thought that she was now the right size for going waited for a few minutes to see if she was going to shrink any further: said Alice to herself, “in my going out altogether, like a candle. I After a while, finding that nothing more happened, she decided on going Rabbit came near her, she began, in a low, timid voice, “If you please, to repeat it, but her voice sounded hoarse and strange, and the words now about two feet high, and was going on shrinking rapidly: she soon dropped it hastily, just in time to avoid shrinking away altogether. “Not like cats!” cried the Mouse, in a shrill, passionate voice. “Would trembling voice, “Let us get to the shore, and then I’ll tell you my “What I was going to say,” said the Dodo in an offended tone, “was, “But who is to give the prizes?” quite a chorus of voices asked. “Why, _she_, of course,” said the Dodo, pointing to Alice with one The next thing was to eat the comfits: this caused some noise and the others all joined in chorus, “Yes, please do!” but the Mouse only doesn’t suit my throat!” and a Canary called out in a trembling voice doing out here? Run home this moment, and fetch me a pair of gloves and at once in the direction it pointed to, without trying to explain the “How queer it seems,” Alice said to herself, “to be going messages for gloves, and was just going to leave the room, when her eye fell upon a she heard a voice outside, and stopped to listen. “Mary Ann! Mary Ann!” said the voice. “Fetch me my gloves this moment!” Next came an angry voice—the Rabbit’s—“Pat! Pat! Where are you?” And then a voice she had never heard before, “Sure then I’m here! Digging rumbling of little cartwheels, and the sound of a good many voices all then the Rabbit’s voice along—“Catch him, you by the hedge!” then silence, and then another confusion of voices—“Hold up his head—Brandy Last came a little feeble, squeaking voice, (“That’s Bill,” thought “We must burn the house down!” said the Rabbit’s voice; and Alice addressed her in a languid, sleepy voice. came different!” Alice replied in a very melancholy voice. By the use of this ointment—one shilling the box— going to dive in among the leaves, which she found to be nothing but Pigeon, raising its voice to a shriek, “and just as I was thinking I going to be, from one minute to another! However, I’ve got back to my are; secondly, because they’re making such a noise inside, no one could noise going on within—a constant howling and sneezing, and every now “Oh, _please_ mind what you’re doing!” cried Alice, jumping up and down (In which the cook and the baby joined): ear and left foot, so as to prevent its undoing itself,) she carried it No, there were no tears. “If you’re going to turn into a pig, my dear,” eyes, “Of course, of course; just what I was going to remark myself.” quarrelled last March—just before _he_ went mad, you know—” (pointing hoarse, feeble voice: “I heard every word you fellows were saying.” The Dormouse had closed its eyes by this time, and was going off into a neither of the others took the least notice of her going, though she voice, “Why the fact is, you see, Miss, this here ought to have been a you see, Miss, we’re doing our best, afore she comes, to—” At this “And who are _these?_” said the Queen, pointing to the three gardeners “Get up!” said the Queen, in a shrill, loud voice, and the three turning to the rose-tree, she went on, “What _have_ you been doing “May it please your Majesty,” said Two, in a very humble tone, going “Come on, then!” roared the Queen, and Alice joined the procession, “It’s—it’s a very fine day!” said a timid voice at her side. She was “Get to your places!” shouted the Queen in a voice of thunder, and had got its neck nicely straightened out, and was going to give the going to begin again, it was very provoking to find that the hedgehog “How do you like the Queen?” said the Cat in a low voice. “Who _are_ you talking to?” said the King, going up to Alice, and Alice thought she might as well go back, and see how the game was going on, as she heard the Queen’s voice in the distance, screaming with a large crowd collected round it: there was a dispute going on between thing before, and he wasn’t going to begin at _his_ time of life. startled when she heard her voice close to her ear. “You’re thinking “The game’s going on rather better now,” she said, by way of keeping up But here, to Alice’s great surprise, the Duchess’s voice died away, “A fine day, your Majesty!” the Duchess began in a low, weak voice. about half no time! Take your choice!” The Duchess took her choice, and was gone in a moment. As they walked off together, Alice heard the King say in a low voice, sea. The master was an old Turtle—we used to call him Tortoise—” “Why did you call him Tortoise, if he wasn’t one?” Alice asked. “We called him Tortoise because he taught us,” said the Mock Turtle Fainting in Coils.” two sobs choked his voice. “Same as if he had a bone in his throat,” the back. At last the Mock Turtle recovered his voice, and, with tears “Change lobsters again!” yelled the Gryphon at the top of its voice. Turtle, suddenly dropping his voice; and the two creatures, who had “There’s a porpoise close behind us, and he’s treading on my tail. They are waiting on the shingle—will you come and join the dance? Will you, won’t you, will you, won’t you, will you join the dance? Will you, won’t you, will you, won’t you, won’t you join the dance? Said he thanked the whiting kindly, but he would not join the dance. Would not, could not, would not, could not, would not join the dance. Would not, could not, would not, could not, could not join the dance. Then turn not pale, beloved snail, but come and join the dance. Will you, won’t you, will you, won’t you, will you join the dance? Will you, won’t you, will you, won’t you, won’t you join the dance?” “Boots and shoes under the sea,” the Gryphon went on in a deep voice, running on the song, “I’d have said to the porpoise, ‘Keep back, wise fish would go anywhere without a porpoise.” and told me he was going a journey, I should say ‘With what porpoise?’” Alice a little timidly: “but it’s no use going back to yesterday, “Stand up and repeat ‘’_Tis the voice of the sluggard_,’” said the “’Tis the voice of the Lobster; I heard him declare, His voice has a timid and tremulous sound.] wrong, and she went on in a trembling voice:— The Mock Turtle sighed deeply, and began, in a voice sometimes choked they doing?” Alice whispered to the Gryphon. “They can’t have anything “Stupid things!” Alice began in a loud, indignant voice, but she “I’m a poor man, your Majesty,” the Hatter began, in a trembling voice, The King looked anxiously at the White Rabbit, who said in a low voice, nearly out of sight, he said in a deep voice, “What are tarts made of?” “Treacle,” said a sleepy voice behind her. little voice, the name “Alice!” “The trial cannot proceed,” said the King in a very grave voice, “until verdict,” he said to the jury, in a low, trembling voice. “Why, there they are!” said the King triumphantly, pointing to the “Off with her head!” the Queen shouted at the top of her voice. Nobody looking up into hers—she could hear the very tones of her voice, and shared their never-ending meal, and the shrill voice of the Queen cries to the voice of the shepherd boy—and the sneeze of the baby, the shriek of the Gryphon, and all the other queer noises, would change unenforceability of any provision of this agreement shall not void the)
// mini exit code = 127
// bash exit code = 0
// mini error = ( No such file or directory)
// bash error = ()
// [0;33mTest   3: ❌[38;5;244m cat minishell.h | grep ");"$ 
// [0mmini exit code = 127
// bash exit code = 1
// [0;33mTest   4: ❌[38;5;244m export GHOST=123 | env | grep GHOST 
// mini exit code = 127
// bash exit code = 1
// mini error = ( No such file or directory)
// bash error = ()



////int	handle_redir_out(t_cmd *cmd, t_data *data)  //bool has_child
////{
////	//t_redir_out	*redir;
////	int			res;

////	redir = cmd->redir_struct;
////	while (redir)
////	{
////		if (redir->type == T_R_OUT)
////			res = redir_out(redir, mem, has_child);
////		else if (redir->type == T_OUT_APPEND)
////			res = redir_append(redir, mem, has_child);
////		if (res == 0)
////			return (0);
////		redir = redir->next;
////	}
////	return (1);
////}

////int	handle_redir_in(t_cmd *cmd, t_data *data)
////{
////	//t_redir_out	*redir;
////	int			res;

////	redir = cmd->redir_struct;
////	while (redir)
////	{
////		if (redir->type == T_R_IN)
////			res = redir_in(cmd, redir, mem, has_child);
////		else if (redir->type == T_HEREDOC)
////			res = redir_heredoc(cmd, redir, mem, has_child);
////		if (res == 0)
////			return (0);
////		redir = redir->next;
////	}
////	return (1);
////}

//pid_t	execute_first_command(t_cmd **cmd, t_data **data,char **env, int pipe_fd[2])
//{
//	(void)data;
//	//dup2(pipe_fd[1], STDOUT_FILENO);
//    //close(pipe_fd[0]);
//    //close(pipe_fd[1]);
//	  if (pipe(pipe_fd) == -1)
//    {
//        perror("pipe");
//        return (-1);
//    }
//    pid_t pid = fork();
//    if (pid < 0)
//    {
//        perror("fork");
//        return (-1);
//    }

//    if (pid == 0)
//    {
//        // CHILD
//        dup2(pipe_fd[1], STDOUT_FILENO);
//        close(pipe_fd[0]);
//        close(pipe_fd[1]);
//        // Example: if cmd->args = ["ls", "-l", NULL],
//        // you might need the full path or do a PATH search.
//        execve((*cmd)->args[0], (*cmd)->args, env);
//        //perror("execve");
//        exit(127);
//    }
//    else
//    {
//        close(pipe_fd[1]);
//    }
//    return (pid);
//}
//pid_t	execute_middle_command(t_cmd **cmd, t_data **data,char **env, int old_pipe_fd[2], int new_pipe_fd[2])
//{
//	(void)data;
//    if (pipe(new_pipe_fd) == -1)
//    {
//        perror("pipe");
//        return (-1);
//    }
//    pid_t pid = fork();
//    if (pid < 0)
//    {
//        perror("fork");
//        return (-1);
//    }

//    if (pid == 0)
//    {
//        // CHILD
//        dup2(old_pipe_fd[0], STDIN_FILENO);
//        dup2(new_pipe_fd[1], STDOUT_FILENO);
//        close(old_pipe_fd[0]);
//        close(old_pipe_fd[1]);
//        close(new_pipe_fd[0]);
//        close(new_pipe_fd[1]);
//        execve((*cmd)->args[0], (*cmd)->args, env);
//        //perror("execve");
//        exit(127);
//    }
//    else
//    {
//        // PARENT
//        close(old_pipe_fd[0]);
//        close(old_pipe_fd[1]);
//        close(new_pipe_fd[1]);
//    }
//    return (pid);
//}

// pid_t	execute_last_command(t_cmd **cmd, t_data **data,char **env, int pipe_fd[2])
// {
//	(void)data;
//    pid_t pid = fork();
//    if (pid < 0)
//    {
//        perror("fork");
//        return (-1);
//    }

//    if (pid == 0)
//    {
//        // CHILD
//        dup2(pipe_fd[0], STDIN_FILENO);
//        close(pipe_fd[0]);
//        close(pipe_fd[1]);
//        execve((*cmd)->args[0], (*cmd)->args, env);
       /// perror("execve");
//        exit(127);
//    }
//    else
//    {
//        // PARENT
//        close(pipe_fd[0]);
//        close(pipe_fd[1]);
//    }
//    return (pid);
// }

////void execute_pipeline(t_cmd **cmd, t_data *data, t_token *tokens, char **env)
////{
////	(void)env;
////	(void)tokens;
////    int pipe_fd[2];
////	//int pipe_fd2[2];
////    int i = 0;
////    //pid_t *pid;
////    int status;

////    if (!cmd || !cmd[0] || !cmd[0]->args)
////    {
////        fprintf(stderr, "minishell: syntax error near unexpected token `|'\n");
////        data->exit_status = 1;
////        return;
////    }
////	if (cmd[i + 1])
////	{
////		execute_first_command(cmd, &data, env, pipe_fd);
////	}
////    cmd = cmd->next;
////	while (cmd->next)
////	{
////		execute_middle_command(cmd, &data, env, pipe_fd);
////		cmd = cmd->next;
////	}
////	execute_last_command(cmd, &data, env, pipe_fd);
////	close(pipe_fd[0]);
////	close(pipe_fd[1]);
////	//handle_multiprocess(data, i);
////}

//int count_commands(t_cmd *cmd)
//{
//    int count = 0;

//    while (cmd[count].args != NULL)
//    {
//        count++;
//    }
//    return count;
//}

//void execute_pipeline(t_cmd **cmd, t_data *data, char **env)
//{

//    int n_cmds = count_commands(*cmd);
//    if (n_cmds == 0)
//        return;
    
//    pid_t pids[n_cmds];
//    int old_pipe_fd[2]; 
//    int new_pipe_fd[2]; 

//    t_cmd *current_cmd = cmd[0];
//    pids[0] = execute_first_command(cmd, &data, env, old_pipe_fd);

//    int i = 1;
//    for (; i < n_cmds - 1; i++)
//    {
//        current_cmd = cmd[i];
//        pids[i] = execute_middle_command(&current_cmd, &data, env, old_pipe_fd, new_pipe_fd);
//        old_pipe_fd[0] = new_pipe_fd[0];
//        old_pipe_fd[1] = new_pipe_fd[1];
//    }
	
//    current_cmd = cmd[i];
//    pids[i] = execute_last_command(&current_cmd, &data, env, old_pipe_fd);


//    int status;
//    for (i = 0; i < n_cmds; i++)
//    {
//        waitpid(pids[i], &status, 0);
//        if (WIFEXITED(status))
//            data->exit_status = WEXITSTATUS(status);
//    }
//}

#include "../minishell.h"

char *find_command_path(const char *cmd, char **env)
{
    int i = 0;
    char *path_env = NULL;
    char *token = NULL;
    char *full_path = NULL;
    size_t len_dir, len_cmd;
    
    // Find the PATH variable in env
    while (env[i])
    {
        if (strncmp(env[i], "PATH=", 5) == 0)
        {
            path_env = env[i] + 5; // Skip "PATH="
            break;
        }
        i++;
    }
    
    if (!path_env)
    {
        // If no PATH is found, we can't search.
        return NULL;
    }
    
    // Duplicate the PATH variable so strtok doesn't modify the original.
    char *paths = strdup(path_env);
    if (!paths)
    {
        perror("strdup");
        return NULL;
    }
    
    len_cmd = strlen(cmd);
    token = strtok(paths, ":");
    while (token)
    {
        len_dir = strlen(token);
        // Allocate memory for full path: directory + "/" + command + '\0'
        full_path = malloc(len_dir + 1 + len_cmd + 1);
        if (!full_path)
        {
            perror("malloc");
            free(paths);
            return NULL;
        }
        // Build the full path string
        strcpy(full_path, token);
        strcat(full_path, "/");
        strcat(full_path, cmd);
        
        // Check if the file exists and is executable.
        if (access(full_path, X_OK) == 0)
        {
            free(paths);
            return full_path; // Caller must free this string later.
        }
        
        free(full_path);
        full_path = NULL;
        
        // Get the next directory.
        token = strtok(NULL, ":");
    }
    
    free(paths);
    return NULL; // Command not found in any PATH directory.
}
int count_commands(t_cmd *cmd)
{
    int count = 0;

    while (cmd[count].args != NULL)
    {
        count++;
    }
    return count;
}

pid_t execute_first_command(t_cmd **cmd, t_data **data, char **env, int pipe_fd[2])
{
	(void)data;
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (-1);
    }

    if (pid == 0) // CHILD
    {
        dup2(pipe_fd[1], STDOUT_FILENO); // Redirect output to the pipe
        close(pipe_fd[0]);
        close(pipe_fd[1]);

		//char *cmd_path = NULL;
        //if (strchr((*cmd)->args[0], '/'))
        //    cmd_path = (*cmd)->args[0];
        //else
        //    cmd_path = find_command_path((*cmd)->args[0], env);
        
        //if (!cmd_path)
        //{
        //    fprintf(stderr, "minishell: %s: command not found\n", (*cmd)->args[0]);
        //    exit(127);
        //}

        execve((*cmd)->args[0], (*cmd)->args, env);
        perror("execve"); // Error in case execve fails
        exit(127);
    }
    else // PARENT
    {
        close(pipe_fd[1]); // Close the write end of the pipe
    }
    return (pid);
}

pid_t execute_middle_command(t_cmd **cmd, t_data **data, char **env, int old_pipe_fd[2], int new_pipe_fd[2])
{
	(void)data;
    if (pipe(new_pipe_fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (-1);
    }

    if (pid == 0) // CHILD
    {
        dup2(old_pipe_fd[0], STDIN_FILENO); // Read from the old pipe
        dup2(new_pipe_fd[1], STDOUT_FILENO); // Write to the new pipe
        close(old_pipe_fd[0]);
        close(old_pipe_fd[1]);
        close(new_pipe_fd[0]);
        close(new_pipe_fd[1]);
        execve((*cmd)->args[0], (*cmd)->args, env);
        //perror("execve");
        exit(127);
    }
    else // PARENT
    {
        close(old_pipe_fd[0]); // Close the old pipe in the parent
        close(old_pipe_fd[1]);
        close(new_pipe_fd[1]); // Close the write end of the new pipe
    }
    return (pid);
}

pid_t execute_last_command(t_cmd **cmd, t_data **data, char **env, int pipe_fd[2])
{
	(void)data;
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (-1);
    }

    if (pid == 0) // CHILD
    {
        dup2(pipe_fd[0], STDIN_FILENO); // Read from the pipe
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        
        // Special handling for echo | echo
        if ((*cmd)->args[0] && strcmp((*cmd)->args[0], "echo") == 0)
        {
            char buffer[1024];
            ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0)
            {
                buffer[bytes_read] = '\0'; // Null-terminate the buffer
                write(STDOUT_FILENO, buffer, bytes_read); // Write the content
            }
            else
            {
                write(STDOUT_FILENO, "\n", 1); // Just print a new line
            }
            exit(0); // Exit after handling echo
        }

        execve((*cmd)->args[0], (*cmd)->args, env);
        //perror("execve");
        exit(127);
    }
    else // PARENT
    {
        close(pipe_fd[0]); // Close the read end of the pipe in the parent
        close(pipe_fd[1]);
    }
    return (pid);
}

void execute_pipeline(t_cmd **cmd, t_data *data, char **env)
{
    int n_cmds = count_commands(*cmd);
    if (n_cmds == 0)
        return;

    pid_t pids[n_cmds];
    int old_pipe_fd[2];
    int new_pipe_fd[2];

    t_cmd *current_cmd = cmd[0];
    pids[0] = execute_first_command(cmd, &data, env, old_pipe_fd);

    int i = 1;
    for (; i < n_cmds - 1; i++)
    {
        current_cmd = cmd[i];
        pids[i] = execute_middle_command(&current_cmd, &data, env, old_pipe_fd, new_pipe_fd);
        old_pipe_fd[0] = new_pipe_fd[0];
        old_pipe_fd[1] = new_pipe_fd[1];
    }

    current_cmd = cmd[i];
    pids[i] = execute_last_command(&current_cmd, &data, env, old_pipe_fd);

    int status;
    for (i = 0; i < n_cmds; i++)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
            data->exit_status = WEXITSTATUS(status);
    }
}







/////////////////////////////////////////////////////////////////////////////

//pid_t execute_last_command(t_cmd **cmd, t_data **data, char **env, int pipe_fd[2])
//{
//    (void)data;
//    pid_t pid = fork();
//    if (pid < 0)
//    {
//        perror("fork");
//        return (-1);
//    }

//    if (pid == 0) // CHILD process
//    {
//        dup2(pipe_fd[0], STDIN_FILENO); // Встановлюємо вхідні дані з пайпа
//        close(pipe_fd[0]);
//        close(pipe_fd[1]);

//        execve((*cmd)->args[0], (*cmd)->args, env);
//        perror("execve"); // Якщо execve не вдасться, виводимо помилку
//        exit(127);
//    }
//    else
//    {
//        // PARENT
//        close(pipe_fd[0]);
//        close(pipe_fd[1]);
//    }
//    return (pid);
//}



//void execute_pipeline(t_cmd **cmd, t_data *data, char **env)
//{
//    int n_cmds = count_commands(*cmd);
//    if (n_cmds == 0)
//        return;
    
//    pid_t pids[n_cmds];
//    int old_pipe_fd[2]; 
//    int new_pipe_fd[2]; 

//    t_cmd *current_cmd = cmd[0];
//    pids[0] = execute_first_command(cmd, &data, env, old_pipe_fd); // Перша команда

//    int i = 1;
//    for (; i < n_cmds - 1; i++)
//    {
//        current_cmd = cmd[i];
//        pids[i] = execute_middle_command(&current_cmd, &data, env, old_pipe_fd, new_pipe_fd); // Середні команди
//        old_pipe_fd[0] = new_pipe_fd[0];
//        old_pipe_fd[1] = new_pipe_fd[1];
//    }
    
//    current_cmd = cmd[i];
//    pids[i] = execute_last_command(&current_cmd, &data, env, old_pipe_fd); // Остання команда

//    int status;
//    for (i = 0; i < n_cmds; i++)
//    {
//        waitpid(pids[i], &status, 0);
//        if (WIFEXITED(status))
//            data->exit_status = WEXITSTATUS(status);
//    }
//}
