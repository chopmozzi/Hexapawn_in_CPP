# Hexapawn_in_CPP
## Introduction
- c++로 작성된 Hexapawn입니다.
- 3x3의 체스판에서 흑, 백 각각 3개의 pawn을 가지고 게임을 진행합니다.
- Player는 직접 원하는 곳으로, Computer는 Alpha-Beta Pruning을 기반으로 움직일 곳을 정합니다.
- Hexapawn에 대한 내용과 세부설계과정은 해당 링크에서 확인할 수 있습니다.
- [Hexapawn(notion)](https://abalone-fahrenheit-80e.notion.site/Hexapawn-81c9292f87de484b9e2d8b9919a01afd?pvs=4)
---
## How to run?
- Visual Studio Community 2022`(Project Build)`로 작성되었습니다.
- Visual Studio Community 프로젝트 내에서 빌드하여 Hexapawn게임을 실행할 수 있습니다.
- 모든 pawn이 벽에 붙어 있는 기본 상태와 txt파일을 이용해 load한 상태 두가지의 상황을 선택할 수 있습니다.
- Player의 Pawn색, 선공 여부를 선택할 수 있습니다.
---
## Example
![load 파일 예시](/image/2.PNG)
- Huffman coindg을 이용하여 압축할 input.txt 예시이다.

![실제 txt파일 예시](/image/1.PNG)
- 실제 메모장을 이용해 로드 파일을 만든 모습이다.

![Default 상태 예시](/image/3.PNG)
- 아무런 파일을 로드 하지 않고 처음 초기 상태의 예시이다.

![파일 load 예시](/image/4.PNG)
- 파일을 load하여 체스판의 상태를 설정한 모습이다.

![Hexapawn play 예시](/image/5.PNG)
- Hexapawn play 예시이다. pawn을 움직여 게임을 진행할 수 있다.
