up:
	wsl --set-default Ubuntu
	wsl -u root apt update
	wsl -u root apt-get upgrade
	wsl -u root apt install -y ntpdate
	wsl -u root ntpdate ntp.nict.jp
	docker compose build --progress=plain
	docker compose up -d

exec:
	docker exec -it mingw bash

up-simply-exec:
	docker compose build --progress=plain
	docker compose up -d

up-exec:
	make up
	make exec
