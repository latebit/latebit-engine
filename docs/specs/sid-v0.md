# SID tune file spefication

This document describes the format for the tune files that libsid can read and play.

While not a standard, this specificaition is intended as the reference for future and
alternative implementations.

<table>
  <tr>
    <th>Status</th><td>Draft</td>
  </tr>
  <tr>
    <th>Version</th><td>v0</td>
  </tr>
  <tr>
    <th>Last Revision</th><td>2024-04-02</td>
  </tr>
</table>

## Updates and versioning

The specification follows a semantic versioning scheme, whereby the first digit is used to 
indicate breaking changes, and the second to indicate backward compatible updates.

Backward compatible updates to this specification will be added to this document and marked 
as updates.

Breaking changes can be found at `spec-vN.md` where `N` is the major version.

## Jargon
* **Tune** 
  
  Any audio (music composition or sound effect) that can be represented by a tune file.

* **BPM**
  
  Beats per minute, the conventional tempo unit used in music.

* **Note**

  A sound that can be represented by a _Symbol_. It can be either a musical note or noise.

* **Tick**

  Equally sized portions of a beat. They are the smallest unit of time in a _Track_, 
  namely a _Note_ can only span one or more ticks.

* **Track**

  A list of _Symbols_ representing a musical sequence.

* **Symbol**

  A string of 6 ASCII characters encoding a _Note_


## Specification
A tune is a text file that looks like this

```
#v0.1#
120
4
3
2
C-10FA|C-10FA
......|------
------|C-20FA
```

### Header
The first 5 lines represent the file header.

#### Version
The first line represent the version of the manifest. It's used by the interpreters to 
know which parsing rules to apply while reading the file.

#### Tempo
The second line represents the tempo of the tune. It's expressed in _BPM_.

#### Ticks Per Beat
The third line indicates the number of _Ticks_ per beat. This number is used to calculate
the speed of the tune.

For example
* 60 BPM and 1 tick per beat means 1 note will be played every 1 second
* 60 BPM and 2 ticks per beat means 1 note will be played every 500ms
* 120 BPM and 3 tick per beat means 1 note will be played every 333.33ms

This number represents the smallest number of notes that can be played per beat and can
be used to indicate simple and compound time signatures: a multiple of 2 yields a simple
meter, a multiple of 3 a compound one.

#### Number of beats
The fourth line represents how many beats a track has in total. Every track will have 
at most `beats * ticks per beat` _Ticks_ in total.

#### Number of tracks
The fifth and last line of the header holds the total number of tracks in the body.

### Body

The remainder of the file is its body.

Similarly to classic tracker softwares, every column of the body represent a _Track_.

The columns are `|`-separated and elements on the same row are executed at the
same time. Last `|` is optional. 

```
C-10FA|C-10FA
......|------
------|
```

Every cell is a _Symbol_.

#### Symbol

Symbols are made of 6 ASCII characters. 

The character `-` (NULL) is used to make certain values fallback to default values. Default values 
are chosen so that users need to input the least amount of characters to hear a sound.

The character `.` (CONTINUE) is a continuation character. It's only used in the continuation Special Symbol.

The character ` ` (END OF TRACK) is only used in the end of track Special Symbol.

There are two type of symbols: Standard Symbol and Special Symbol.

##### Standard Symbol

A Standard Symbol looks like

```
Eb30F1
```

The meaning of the characters by their zero-based index is the following

| Index 	| Description                                                               	| Allowed Values                                               	    | Default         	|
|-------	|---------------------------------------------------------------------------	|-----------------------------------------------------------------	|-----------------	|
| 0     	| Pitch, following the English naming convention.                           	| `A`, `B`, `C`, `D`, `E`, `F`, `G`                            	    | Rest (no sound) 	|
| 1     	| Accidental, according to chromatic semitones.                             	| `b` (-1 semitone)<br>`#` (+1 semitone)                         	  | No accidentals  	|
| 3     	| Octave, in Scientific Pitch Notation                                      	| `0-7`                                                          	  | `4`               |
| 4     	| Waveform, the timbre of the note                                          	| `0` (triangle)<br>`1` (square)<br>`2` (sawtooth)<br>`3` (noise) 	| `0`    	          |
| 5     	| Volume of the note in hexadecimal where 0 is silent and F maximum volume. 	| `0-F` (hexadecimal)                                            	  | `8`               |
| 6     	| Effect applied to the note.                                               	| `0` (no effect)<br>`1` (drop)<br>`2` (slide)<br>`3` (fade in)<br>`4` (fade out) | `0`     |

The Symbol in the example above means: Eb on octave 3 using a triangle wave, at the maximum volume with a slide effect.

##### Special Symbols

There are the following special symbols:

* `------` (6x NULL)
  
  This is a rest. No sound will be produced.

* `......` (6x CONTINUE)

  This is a continue symbol. It will make the previous symbol continue playing over the current tick.

* `      ` (6x SPACE)

  This is a end of track symbol. Should it occur in the middle of a track, the rest of the symbols will be ignored.
