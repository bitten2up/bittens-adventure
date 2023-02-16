/*
* MIT License
*
* Copyright (c) 2023 bitten2up
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/


/////////////////////////////////////////////////////////
//  saving functionality
/////////////////////////////////////////////////////////
//  done in rust to keep shit safe (kinda not really)
/////////////////////////////////////////////////////////

#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(dead_code)]

mod bittendef;


// i will define this here instead of using bindgen
// so then i have control over it
use std::path::Path;
extern crate savefile;
use savefile::prelude::*;

#[macro_use]
extern crate savefile_derive;

#[derive(Savefile)]
pub struct bitSave {
    // first 7 bytes is the header, used to make sure that we are using the right type of file
    header:     [i8;8],
    // the next 4 bytes is the version
    version:    u32,
    // the next 2 bytes is our x position
    xpos:       i16,
    // the next 2 bytes is our y position
    ypos:       i16,
    // the next bit is to see if music is enabled
    music:      bool,
}

fn reset_save() -> bitSave {
    println!("reset save");
    return bitSave { header: [0x42, 0x49, 0x54, 0x53, 0x41, 0x56, 0x00, 0x7f], version: bittendef::BIT_VERSION, xpos: 0, ypos: 0, music: true };
}


#[no_mangle]
pub extern "C" fn saveGame(game_state: *mut bittendef::bit_game){
    println!("savegame");
    unsafe
    {
        save_data(bitSave { header: [0x42, 0x49, 0x54, 0x53, 0x41, 0x56, 0x00, 0x7f],
            version: bittendef::BIT_VERSION,
            xpos: (*game_state).player.x,
            ypos: (*game_state).player.y,
            music: (*game_state).settings.audio});
    }
}

fn save_data(save_data: bitSave) {
    save_file("bitten.sav", 0, &save_data).expect("bitten.sav could not be written to.\nIs it a folder?\nDo you have write access?\nDid you try to write to a folder that does not expect");
}

#[no_mangle]
pub extern "C" fn loadGame(game_state: *mut bittendef::bit_game) {
    println!("load game");
    let mut save_data;
    save_data = load_data();
    if save_data.header != [0x42, 0x49, 0x54, 0x53, 0x41, 0x56, 0x00, 0x7f] {
        save_data = reset_save();
    }
    updateState(game_state, save_data);
}
fn load_data() -> bitSave {
    // prevent crash by seeing if file is real
    if Path::new("./bitten.sav").exists() {
        return load_file("bitten.sav", 0).expect("damm you made a file race condition");
    }
    else {
        return reset_save();
    }
}
// "safe" wrapper for writing saves to game_state
fn updateState(game_state: *mut bittendef::bit_game, save_data: bitSave) {
    // see if our version is higher than the current version
    if save_data.version > bittendef::BIT_VERSION {
        unsafe {(*game_state).invalidSave=true;}
    }
    unsafe {
        (*game_state).player.x = save_data.xpos;
        (*game_state).player.y = save_data.ypos;
        (*game_state).settings.audio = save_data.music;
    }
}