# Ops Workflow

This folder keeps Codex workflow state stable across sessions.

## Structure

- `tasks/TODO.md`: prioritized backlog
- `notes/SESSION_LOG.md`: append-only session history
- `state/CURRENT_STATE.md`: latest generated snapshot
- `roadmap/PHASES.md`: path to full TP/TWW/MP4-style pipeline

## Update command

Run after meaningful progress:

```powershell
python tools/update_ops_state.py --summary "what changed" --next "next task"
```

Optional second next step:

```powershell
python tools/update_ops_state.py --summary "..." --next "..." --next "..."
```
