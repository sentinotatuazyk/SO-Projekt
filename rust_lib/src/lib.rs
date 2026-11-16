use crossterm::{
    event::{self, Event, KeyCode},
    terminal::{disable_raw_mode, enable_raw_mode, EnterAlternateScreen, LeaveAlternateScreen},
    ExecutableCommand,
};

use ratatui::{
    backend::CrosstermBackend,
    layout::{Alignment, Constraint, Direction, Layout},
    style::{Color, Style},
    widgets::{Block, Borders, Paragraph},
    Terminal,
};
use std::{
    io::{self, stdout},
    time::{Duration, Instant},
};

const FRAME_1: &str = r#"
           ________________________
         ///\\//\\//\\//\\//\\//\\/\\\
       //\{}//\\//\\//\\//\\//\\//{}/\\
      ///&%&%&%&/~~~~~~~~~~~~\&%&%&%&\\\
      ||&%&%&_.'              '._&%&%&||
      ||&%'''                    '''%&||
      ||&%&                ~c      &%&||
      ||&%&     o   )____  <)\/    &%&||
      ||&%&    /_=-/_____|  /\     &%&||
      ||&%&&  ( /\.|     | /~/    &&%&||
______||&%&&======================&&%&||______
"#;

const FRAME_2: &str = r#"
           ________________________
         ///\\//\\//\\//\\//\\//\\/\\\
       //\{}//\\//\\//\\//\\//\\//{}/\\
      ///&%&%&%&/~~~~~~~~~~~~\&%&%&%&\\\
      ||&%&%&_.'              '._&%&%&||
      ||&%'''                    '''%&||
      ||&%&                 ~c     &%&||
      ||&%&     o   )____   <)\/   &%&||
      ||&%&    /_=_/_____|   /\    &%&||
      ||&%&&  ( /\.|     |  /~/   &&%&||
______||&%&&======================&&%&||______
"#;

const FRAME_3: &str = r#"
           ________________________
         ///\\//\\//\\//\\//\\//\\/\\\
       //\{}//\\//\\//\\//\\//\\//{}/\\
      ///&%&%&%&/~~~~~~~~~~~~\&%&%&%&\\\
      ||&%&%&_.'              '._&%&%&||
      ||&%'''                    '''%&||
      ||&%&           ♩      ~c    &%&||
      ||&%&     o   )____    <)\/  &%&||
      ||&%&    /_=_/_____|    /\   &%&||
      ||&%&&  ( /\.|     |   /~/  &&%&||
______||&%&&======================&&%&||______
"#;

const FRAME_4: &str = r#"
           ________________________
         ///\\//\\//\\//\\//\\//\\/\\\
       //\{}//\\//\\//\\//\\//\\//{}/\\
      ///&%&%&%&/~~~~~~~~~~~~\&%&%&%&\\\
      ||&%&%&_.'              '._&%&%&||
      ||&%'''          ♩         '''%&||
      ||&%&                    c~  &%&||
      ||&%&     o   )____    \/(>  &%&||
      ||&%&    /_=-/_____|    /\   &%&||
      ||&%&&  ( /\.|     |    \~\ &&%&||
______||&%&&======================&&%&||______
"#;

const FRAME_5: &str = r#"
           ________________________
         ///\\//\\//\\//\\//\\//\\/\\\
       //\{}//\\//\\//\\//\\//\\//{}/\\
      ///&%&%&%&/~~~~~~~~~~~~\&%&%&%&\\\
      ||&%&%&_.'       ♩      '._&%&%&||
      ||&%'''                    '''%&||
      ||&%&          ♩        c~   &%&||
      ||&%&     o   )____   \/(>   &%&||
      ||&%&    /_=_/_____|   /\    &%&||
      ||&%&&  ( /\.|     |   \~\  &&%&||
______||&%&&======================&&%&||______
"#;

const FRAME_6: &str = r#"
           ________________________
         ///\\//\\//\\//\\//\\//\\/\\\
       //\{}//\\//\\//\\//\\//\\//{}/\\
      ///&%&%&%&/~~~~~~~~~~~~\&%&%&%&\\\
      ||&%&%&_.'              '._&%&%&||
      ||&%'''         ♩          '''%&||
      ||&%&                  c~    &%&||
      ||&%&     o   )____  \/(>    &%&||
      ||&%&    /_=_/_____|  /\     &%&||
      ||&%&&  ( /\.|     |  \~\   &&%&||
______||&%&&======================&&%&||______
"#;

const FRAME_7: &str = r#"
           ________________________
         ///\\//\\//\\//\\//\\//\\/\\\
       //\{}//\\//\\//\\//\\//\\//{}/\\
      ///&%&%&%&/~~~~~~~~~~~~\&%&%&%&\\\
      ||&%&%&_.'     ♩        '._&%&%&||
      ||&%'''                    '''%&||
      ||&%&                 c~     &%&||
      ||&%&     o   )____ \/(>     &%&||
      ||&%&    /_=-/_____| /\      &%&||
      ||&%&&  ( /\.|     | \~\    &&%&||
______||&%&&======================&&%&||______
"#;



#[unsafe(no_mangle)]
pub extern "C" fn zrusta() ->io::Result<()> {
    enable_raw_mode()?;
    stdout().execute(EnterAlternateScreen)?;
    let mut terminal = Terminal::new(CrosstermBackend::new(stdout()))?;

    let frames = vec![FRAME_1, FRAME_2, FRAME_3, FRAME_4, FRAME_5 , FRAME_6, FRAME_7];
    let mut current_frame_idx = 0;

    let tickrate = Duration::from_millis(250);
    let mut last_tick=Instant::now();

    loop {
        terminal.draw(|f| {
            let chunks = Layout::default()
                .direction(Direction::Vertical)
                .constraints([
                    Constraint::Min(3), 
                    Constraint::Length(3)
                ])
                .split(f.size());

            let ascii_widget = Paragraph::new(frames[current_frame_idx])
                .alignment(Alignment::Center)
                .style(Style::default().fg(Color::Cyan))
                .block(Block::default().borders(Borders::ALL).title("Bardzo prosze nie usuwac kompilatora :)"));

            let help_widget = Paragraph::new("Nacisnij [Q], aby wyjsc")
                .alignment(Alignment::Center);
            f.render_widget(ascii_widget, chunks[0]);
            f.render_widget(help_widget, chunks[1]);
        })?;

        let timeout = tickrate
            .checked_sub(last_tick.elapsed())
            .unwrap_or(Duration::from_secs(0));

        if event::poll(timeout)? {
            if let Event::Key(key) = event::read()? {
                if key.code == KeyCode::Char('q') || key.code == KeyCode::Char('Q') {
                    break
                }
            }
        }

        if last_tick.elapsed() >= tickrate {
            current_frame_idx = (current_frame_idx + 1) % frames.len();
            last_tick = Instant::now();
        }
    }
    disable_raw_mode()?;
    stdout().execute(LeaveAlternateScreen)?;
    Ok(())
}
